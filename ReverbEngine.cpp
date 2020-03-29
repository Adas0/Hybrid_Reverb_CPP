/*
  ==============================================================================

    ReverbEngine.cpp
    Created: 28 Mar 2020 12:27:48pm
    Author:  Adam

  ==============================================================================
*/

#include "ReverbEngine.h"

void ReverbEngine::prepare(double sampleRate, int samplesPerBlock, int numChannels)
{
	sampleRate_ = sampleRate;
	filterGenerator.prepare(sampleRate, samplesPerBlock, numChannels);
	spatialMaker.prepare();

	const int delayBufferSize = 6 * (sampleRate);
	delayBuffer.setSize(numChannels, delayBufferSize);
	delayBuffer.clear();


	delayTimes.lowDelayTime = 15;
	delayTimes.highDelayTime = 400;
	delayTimesNumber = numberDelayLines;
	delayTimesArray = delayTimes.getDelayTimes(delayTimesNumber, delayTimes.lowDelayTime, delayTimes.highDelayTime);
	//int lateTailArrayIndex = 2 * delayTimesNumber / 3;	


	lateReverb.lateReverbNumLines = delayTimesNumber / 2; //biorę ostatnią 1/3 elementów tablicy zawierającej czasy opóźnienia
														  //i bede podmieniał je na odbicia większe niż zakres czasów opóźnienia zwykłych odbić
	lateReverb.addLateReverb(delayTimesArray);
	//first reflections
	delayTimesArray[0] = 80;
	delayTimesArray[1] = 120;
	delayTimesArray[2] = 140;
}


void ReverbEngine::process(AudioBuffer<float>&buffer)
{
	dsp::AudioBlock<float> block(buffer);

	const int bufferLength = buffer.getNumSamples();
	const int delayBufferLength = delayBuffer.getNumSamples();

	const float* bufferDataL = buffer.getReadPointer(leftChannel);
	const float* bufferDataR = buffer.getReadPointer(rightChannel);
	const float* delayBufferDataL = delayBuffer.getReadPointer(leftChannel);
	const float* delayBufferDataR = delayBuffer.getReadPointer(rightChannel);


	float* dryBufferL = buffer.getWritePointer(leftChannel);
	float* dryBufferR = buffer.getWritePointer(rightChannel);

	for (int channel = 0; channel < numInputChannels; ++channel)
	{
		const float* bufferData = buffer.getReadPointer(channel);
		const float* delayBufferData = delayBuffer.getReadPointer(channel);
		copyBufferToDelayBuffer(channel, bufferData, delayBufferData, bufferLength, delayBufferLength);
	}
	
	for (int filter = 0; filter < filterGenerator.filtersNumber; ++filter)
	{
		copyBackToCurrentBuffer(buffer, leftChannel, bufferDataL, delayBufferDataL, bufferLength, delayBufferLength, delayTimesArray[filter]);
		copyBackToCurrentBuffer(buffer, rightChannel, bufferDataR, delayBufferDataR, bufferLength, delayBufferLength, delayTimesArray[filter] + spatialMaker.ITDCoefficients[filter]);



		filterGenerator.lowPassFilter[filter].process(dsp::ProcessContextReplacing<float>(block));
		addDelayWithCurrentBuffer(leftChannel, bufferLength, delayBufferLength, dryBufferL, delayTimesNumber);
		addDelayWithCurrentBuffer(rightChannel, bufferLength, delayBufferLength, dryBufferR, delayTimesNumber);
	}

	//w tym miejscu będę dodawał późne odbicia o zwiększonej amplitudzie. Czyli taka sama pętla jak powyżej

	filterGenerator.allPassFilter.process(dsp::ProcessContextReplacing<float>(block));

	copyBackToCurrentBuffer(buffer, leftChannel, bufferDataL, delayBufferDataL, bufferLength, delayBufferLength, 0);
	copyBackToCurrentBuffer(buffer, rightChannel, bufferDataR, delayBufferDataR, bufferLength, delayBufferLength, 0);

	addDelayWithCurrentBuffer(leftChannel, bufferLength, delayBufferLength, dryBufferL, delayTimesNumber);
	addDelayWithCurrentBuffer(rightChannel, bufferLength, delayBufferLength, dryBufferR, delayTimesNumber);


	bufferWritePosition += bufferLength; //przesuwamy pozycjê czyli miejsce do którego wklejamy nasz buffer
								//czyli je¿eli bufor ma 512 próbek to kolejmy wklejamy w miejsce od indeksu 513
	bufferWritePosition = bufferWritePosition % delayBufferLength;	//je¿eli pozycja wpisania wykracza poza d³ugoœæ bufora to znowu idzie na pocz¹tek:
															//np. bufor opóŸniaj¹cy to 2048. pozycja wpisania to 2048, dziêki modulo pozycja idzie znów na 
															//pocz¹tek czyli na 0, potem 1
															//mWritePosition to pole klasy - bêdziemy tego indeksu u¿ywaæ te¿ przy czytaniu z bufora

}


void ReverbEngine::copyBufferToDelayBuffer(int channel, const float* bufferData, const float* delayBufferData,
	const int bufferLength, const int delayBufferLength)
{
	//kopiujemy dane z naszego buffera do delay buffera

	//sprawdzenie, czy dlugosc bufora opozniajacego jest wieksza niz d³ugoœæ bufora + pozycja. czyli 
	if (delayBufferLength > bufferLength + bufferWritePosition)	//mo¿e byc taka sytuacja - np bufor ma dlugosc 512, bufor delay 2048, pozycja to 1537 (3* 512)
															//czyli 1537 + 512 > 2038 (jest równe 2049)? cos takiego, nie rozumiem do koñca
															//wiêc musimy w tej sytuacji przesuwamy na dlugosc bufora opozniajacego minus pozycja 
	{
		delayBuffer.copyFromWithRamp(channel, bufferWritePosition, bufferData, bufferLength, 1, 1);
	}
	else
	{
		const int bufferRemaning = delayBufferLength - bufferWritePosition; //index 
		delayBuffer.copyFromWithRamp(channel, bufferWritePosition, bufferData, bufferRemaning, 1, 1);

		delayBuffer.copyFromWithRamp(channel, 0, bufferData, bufferLength - bufferRemaning, 1, 1);	//to co zosta³o wklejamy na pocz¹tek. 
																										//czyli to co zosta³o odciête (ca³y bufor to 
																										//bufferRemaining + ta reszta któr¹ tu doklejamy
	}
}


void ReverbEngine::copyBackToCurrentBuffer(AudioBuffer<float>& buffer, int channel, const float* bufferData, const float* delayBufferData,
	const int bufferLength, const int delayBufferLength, int delayTime)
{
	//int delayTime = 500;
	const int bufferReadPosition = static_cast<int>(delayBufferLength + bufferWritePosition - (sampleRate_ * delayTime / 1000)) % delayBufferLength;
	//ile próbek dalej bêdziemy 'wklejaæ' sygna³. 
	//wiêc musi byæ to int. Modulo znów ¿eby 'zawin¹æ' sygna³
	//w tym buforze - indeks idzie na pocz¹tek
	//ten static_cast<int> to po prostu rzutowanie na int, tak 
	//samo jakby napisaæ (int)zmienna. ¿eby upewniæ siê, ¿e 
	//liczymy modulo z inta (bo to index wiêc musi byæ intem)
	//jest plus mWritePosition ¿ebyœmy siê przesuwali po prostu 
	//i czytali za ka¿dym razem mWritePosition dalej (bo wczytujemy 
	//te¿ przesuwaj¹c siê o taki krok)




	if (delayBufferLength > bufferLength + bufferReadPosition)			//ten if to analogicznie jak przy czytaniu - dopóku jest wystarczaj¹ca iloœæ próbek 
																		//w buforze opóŸniaj¹cym ¿eby wzi¹æ stamt¹d iloœæ próbek równ¹ d³ugoœci naszego bufora,
																		//to dodajemy kawa³ek o d³ugoœci naszego bufora (z przesz³oœci) do naszego bufora
	{
		buffer.copyFromWithRamp(channel, 0, delayBufferData + bufferReadPosition, bufferLength, 1, 1);
	}
	else																//je¿eli nie mamy tyle próbek (bufor opóŸniaj¹cy siê 'koñczy') to bierzemy kawa³ek z koñca
																		//i resztê z pocz¹tku 
	{
		const int bufferRemaining = delayBufferLength - bufferReadPosition;		//ile wartoœci zosta³o w delay bufferze
		buffer.copyFromWithRamp(channel, 0, delayBufferData + bufferReadPosition, bufferRemaining, 1, 1);
		buffer.copyFromWithRamp(channel, bufferRemaining, delayBufferData, bufferLength - bufferRemaining, 1, 1);
	}
	//buffer.applyGain(0.9);
}

void ReverbEngine::addDelayWithCurrentBuffer(int channel, const int bufferLength,
	const int delayBufferLength, float* bufferData, int delayTimesNumber)
{ 
	//const float* dryRead = dryBuffer.getReadPointer()
	//float amplitudeMultiplier = 0.68 / (delayTimesNumber);
	float amplitudeMultiplier = 0.85 / (delayTimesNumber);
	//int amplitudeMultiplier_ = Random::getSystemRandom().nextInt(Range<int>(1, 12));
	//float amplitudeMultiplier = (float)amplitudeMultiplier_ / 10;
	//float amplitudeMultiplier = 0.15;
	if (delayBufferLength > bufferLength + bufferWritePosition)
	{

		delayBuffer.addFromWithRamp(channel, bufferWritePosition, bufferData, bufferLength, amplitudeMultiplier, amplitudeMultiplier);		//ostatnie 2 argumenty - jak szybko zanika 
																														// - mno¿enie amplitudy z ka¿dym odbiciem

	}
	else
	{
		const int bufferRemaining = delayBufferLength - bufferWritePosition;
		delayBuffer.addFromWithRamp(channel, bufferWritePosition, bufferData, bufferRemaining, amplitudeMultiplier, amplitudeMultiplier);
		delayBuffer.addFromWithRamp(channel, 0, bufferData, bufferLength - bufferRemaining, amplitudeMultiplier, amplitudeMultiplier);

	}
	//mDelayBuffer.applyGain(0.7);
}