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

	const int delayBufferSize = 4 * (sampleRate);
	delayBuffer.setSize(numChannels, delayBufferSize);
	delayBuffer.clear();


	//delayTimes.lowDelayTime = 10;
	//delayTimes.highDelayTime = 800;
	delayTimesNumber = numberDelayLines;
	delayTimesArray = delayTimes.getDelayTimes(delayTimesNumber, delayTimes.lowDelayTime, delayTimes.highDelayTime);
	//int lateTailArrayIndex = 2 * delayTimesNumber / 3;	

	//lateReverb.lateReverbNumLines =  delayTimesNumber / 2; //biorę ostatnią 1/3 elementów tablicy zawierającej czasy opóźnienia
														  //i bede podmieniał je na odbicia większe niż zakres czasów opóźnienia zwykłych odbić
	//lateReverb.addLateReverb(delayTimesArray);


	delayTimesArray[delayTimesArray.size() - 1] = 0;

	delayTimesArray[0] = 600;
	//spatialMaker.ITDCoefficients[0] = 5; // or -5

	//delayTimesArray[0] = 700;
	//spatialMaker.ITDCoefficients[0] = 30;
	//delayTimesArray[1] = 220;
	//delayTimesArray[2] = 240;

	//amplitudeEarly = 0.98f / delayTimesNumber;
	//amplitudeLate = 0.5f / delayTimesNumber;

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

	//buffer.applyGain(0.9);
	for (int channel = 0; channel < numInputChannels; ++channel)
	{
		const float* bufferData = buffer.getReadPointer(channel);
		const float* delayBufferData = delayBuffer.getReadPointer(channel);
		
		copyBufferToDelayBuffer(channel, bufferData, delayBufferData, bufferLength, delayBufferLength);
	}
	//buffer.applyGain(0.5);
	//delayBuffer.applyGain(0.9);
	//buffer.applyGain(0.1);
	//delayBuffer.applyGain(0.1);
	//kilka 1 ms linii
	//for (int i = 0; i < 5; i++)
	//{
	//	copyBackToCurrentBuffer(buffer, leftChannel, bufferDataL, delayBufferDataL, bufferLength, delayBufferLength, i);
	//	copyBackToCurrentBuffer(buffer, rightChannel, bufferDataR, delayBufferDataR, bufferLength, delayBufferLength, i);

	/*++asd;
	if (asd > 100) asd = 0;

	if (asd % 10 == 0)
		delayBuffer.clear();*/


	//	//filterGenerator.lowPassFilter[filter].process(dsp::ProcessContextReplacing<float>(block));
	//	addDelayWithCurrentBuffer(leftChannel, bufferLength, delayBufferLength, dryBufferL, delayTimesNumber, amplitudeEarly);
	//	addDelayWithCurrentBuffer(rightChannel, bufferLength, delayBufferLength, dryBufferR, delayTimesNumber, amplitudeEarly);
	//}
	
	for (int line = 0; line < delayTimesNumber; ++line)
	{
		/*if (filter < delayTimesNumber - lateReverb.lateReverbNumLines)
		{*/  
		float delayTimeDependantAmp = (1 - (delayTimesArray[line] /*/ delayTimes.highDelayTime) *//  delayTimes.delayTimesPrime[delayTimesNumber + 23]) * 0.65 );
		
		float wet = 1 - wetDry;
		//float delayTimeDependantAmp = 0.4f;
		if (line == delayTimesNumber - 1)	//first reflection
		{
			//wetDry
			copyBackToCurrentBuffer(buffer, leftChannel, bufferDataL, delayBufferDataL, bufferLength, delayBufferLength, delayTimesArray[line]);
			copyBackToCurrentBuffer(buffer, rightChannel, bufferDataR, delayBufferDataR, bufferLength, delayBufferLength, delayTimesArray[line]);




			//filterGenerator.lowPassFilter[0].process(dsp::ProcessContextReplacing<float>(block));
			//tu będzie jeszcze delayTimeDepentantFilter


			
			addDelayWithCurrentBuffer(leftChannel, bufferLength, delayBufferLength, dryBufferL, delayTimesNumber, wetDry);
			addDelayWithCurrentBuffer(rightChannel, bufferLength, delayBufferLength, dryBufferR, delayTimesNumber, wetDry);
			//buffer.applyGain(wetDry);

			//to jest dobrze, tylko trzeba zamienić ten dźwięk bezpośredni na bez sprzężenia. ok chyba jest
			

		} 
		else
		{
			copyBackToCurrentBuffer(buffer, leftChannel, bufferDataL, delayBufferDataL, bufferLength, delayBufferLength, delayTimesArray[line]);
			filterGenerator.lowPassFilterLeft[line].process(dsp::ProcessContextReplacing<float>(block));
			copyBackToCurrentBuffer(buffer, rightChannel, bufferDataR, delayBufferDataR, bufferLength, delayBufferLength, delayTimesArray[line] /*+ spatialMaker.ITDCoefficients[line]*/);

			dsp::AudioBlock<float> block(buffer);
			filterGenerator.lowPassFilterRight[line].process(dsp::ProcessContextReplacing<float>(block));
			addDelayWithCurrentBuffer(leftChannel, bufferLength, delayBufferLength, dryBufferL, delayTimesNumber, delayTimeDependantAmp * wet / (delayTimesNumber - 1));
			addDelayWithCurrentBuffer(rightChannel, bufferLength, delayBufferLength, dryBufferR, delayTimesNumber, (delayTimeDependantAmp + spatialMaker.ILDCoefficients[line] * ILDwet) * wet / (delayTimesNumber - 1));
		} 
		
	}
	//filterGenerator.allPassFilter.process(dsp::ProcessContextReplacing<float>(block));

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
		delayBuffer.copyFromWithRamp(channel, bufferWritePosition, bufferData, bufferLength, 0.9, 0.9);
	}
	else
	{
		const int bufferRemaning = delayBufferLength - bufferWritePosition; //index 
		delayBuffer.copyFromWithRamp(channel, bufferWritePosition, bufferData, bufferRemaning, 0.9, 0.9);

		delayBuffer.copyFromWithRamp(channel, 0, bufferData, bufferLength - bufferRemaning, 0.9, 0.9);	//to co zosta³o wklejamy na pocz¹tek. 
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
		buffer.copyFromWithRamp(channel, 0, delayBufferData + bufferReadPosition, bufferLength, 0.9, 0.9);
	}
	else																//je¿eli nie mamy tyle próbek (bufor opóŸniaj¹cy siê 'koñczy') to bierzemy kawa³ek z koñca
																		//i resztê z pocz¹tku 
	{
		const int bufferRemaining = delayBufferLength - bufferReadPosition;		//ile wartoœci zosta³o w delay bufferze
		buffer.copyFromWithRamp(channel, 0, delayBufferData + bufferReadPosition, bufferRemaining, 0.9, 0.9);
		buffer.copyFromWithRamp(channel, bufferRemaining, delayBufferData, bufferLength - bufferRemaining, 0.9, 0.9);
	}
	//buffer.applyGain(0.9);
}

void ReverbEngine::addDelayWithCurrentBuffer(int channel, const int bufferLength,
	const int delayBufferLength, float* bufferData, int delayTimesNumber, float amplitudeMultiplier)
{ 
	//const float* dryRead = dryBuffer.getReadPointer()
	//float amplitudeMultiplier = 0.68 / (delayTimesNumber);
	//float amplitudeMultiplier = 0.9 / (delayTimesNumber);
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