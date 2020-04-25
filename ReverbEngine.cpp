/*
  ==============================================================================

    ReverbEngine.cpp
    Created: 28 Mar 2020 12:27:48pm
    Author:  Adam

  ==============================================================================
*/

#include "ReverbEngine.h"
#include <math.h>

void ReverbEngine::prepare(double sampleRate, int samplesPerBlock, int numChannels)
{
	sampleRate_ = sampleRate;
	filterGenerator.prepare(sampleRate, samplesPerBlock, numChannels);
	spatialMaker.prepare();

	const int delayBufferSize = 4 * (sampleRate);
	delayBuffer.setSize(numChannels, delayBufferSize);
	delayBuffer.clear();


	delayTimes.lowDelayTime = 2;
	delayTimes.highDelayTime = 800;
	delayTimesNumber = numberDelayLines;
	delayTimesArray = delayTimes.getDelayTimes(delayTimesNumber, delayTimes.lowDelayTime, delayTimes.highDelayTime);
	//int lateTailArrayIndex = 2 * delayTimesNumber / 3;	

	//lateReverb.lateReverbNumLines =  delayTimesNumber / 2; //biorę ostatnią 1/3 elementów tablicy zawierającej czasy opóźnienia
														  //i bede podmieniał je na odbicia większe niż zakres czasów opóźnienia zwykłych odbić
	//lateReverb.addLateReverb(delayTimesArray);
	 

	delayTimesArray[delayTimesArray.size() - 1] = 0;

	//delayTimesArray[0] = 40;
	//delayTimesArray[1] = 173;
	//delayTimesArray[0] = 80;
	//spatialMaker.ITDCoefficients[0] = 5; // or -5
	//delayTimesArray[0] = 700;
	//spatialMaker.ITDCoefficients[0] = 30;
	//delayTimesArray[1] = 220;
	//delayTimesArray[2] = 240;
	//mWetDry /= numberDelayLines;

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

		//buffer.applyGainRamp(channel, 0, bufferLength, 1.0 - (mWetDry /** numberDelayLines*/), 1.0 - (mWetDry /** numberDelayLines*/));
		//buffer.applyGainRamp(channel, 0, bufferLength, 0.2, 0.2);
	}
	
	

	//buffer.applyGain(0.5);
	//delayBuffer.applyGain(0.9);
	//buffer.applyGain(0.1);
	//delayBuffer.applyGain(0.1);
	//kilka 1 ms linii
	//for (int i = 0; i < 5; i++)
	//{
	//	copyBackToCurrentBuffer(buffer, leftChannel, bufferDataL, delayBufferDataL, bufferLength, delayBufferLength, i);
	//buffer.applyGain(0.5);
	//delayBuffer.applyGain(0.9);
	//buffer.applyGain(0.1);
	//delayBuffer.applyGain(0.1);
	//kilka 1 ms linii
	//for (int i = 0; i < 5; i++)
	//{
	//	copyBackToCurrentBuffer(buffer, leftChannel, bufferDataL, delayBufferDataL, bufferLength, delayBufferLength, i);
	//	copyBackToCurrentBuffer(buffer, rightChannel, bufferDataR, delayBufferDataR, bufferLength, delayBufferLength, i);B
	//	copyBackToCurrentBuffer(buffer, rightChannel, bufferDataR, delayBufferDataR, bufferLength, delayBufferLength, i);

	/*++asd;
	if (asd > 100) asd = 0;

	if (asd % 10 == 0)
		delayBuffer.clear();*/


	//	//filterGenerator.lowPassFilter[filter].process(dsp::ProcessContextReplacing<float>(block));
	//	addDelayWithCurrentBuffer(leftChannel, bufferLength, delayBufferLength, dryBufferL, delayTimesNumber, amplitudeEarly);
	//	addDelayWithCurrentBuffer(rightChannel, bufferLength, delayBufferLength, dryBufferR, delayTimesNumber, amplitudeEarly);
	//}
	 
	/*std::vector<int>reverbSizeVector;
	reverbSizeVector.clear();
	for (int line = 0; line < delayTimesNumber; ++line)
	{
		if (line <= reverbSize)
			reverbSizeVector.push_back(delayTimesArray[line]);
		else
			reverbSizeVector.push_back(0);
	}*/
	

	for (int line = 0; line < delayTimesNumber; ++line)
	{
		/*if (filter < delayTimesNumber - lateReverb.lateReverbNumLines)
		{*/  
		//float delayTimeDependantAmp = (1 - (delayTimesArray[line] /*/ delayTimes.highDelayTime) *//  delayTimes.delayTimesPrime[delayTimesNumber + 60]) * 0.7 );
		//float delayTimeDependantAmp = (1 - ((line * 4) * 0.85 / 100)) * 1.009; 
		float amplitude = 0.7;
		/*delayTimesArray[0] = 500;
		delayTimesArray[1] = 50;*/
		//float asd = std::pow(0.05 / (float)delayTimesArray[line], 1 / (float)delayTimesArray[line]);
		//asd = 1.7 - asd;
		//float asd = std::pow(0.05 / 500, 1/500);
		//float delayTimeDependantAmp = asd/delayTimesNumber;
		//float delayTimeDependantAmp = 0.7;
		//float wet = 1 - wetDry;
		//float delayTimeDependantAmp = 0.4f; 
		if (line == delayTimesNumber - 1)	//direct sound
		{  
			////wetDry
			//copyBackToCurrentBuffer(buffer, leftChannel, bufferDataL, delayBufferDataL, bufferLength, delayBufferLength, delayTimesArray[line],/* mWetDry*/ /*1 - */(1 - mWetDry)/* / (float)numberDelayLines*/) /** numberDelayLines*/;
			//copyBackToCurrentBuffer(buffer, rightChannel, bufferDataR, delayBufferDataR, bufferLength, delayBufferLength, delayTimesArray[line],/* mWetDry*/ /*1 -*/ (1 - mWetDry) /*/ (float)numberDelayLines*/) /** numberDelayLines*/;
			copyBackToCurrentBuffer(buffer, leftChannel, bufferDataL, delayBufferDataL, bufferLength, delayBufferLength, delayTimesArray[line], 1);
			copyBackToCurrentBuffer(buffer, rightChannel, bufferDataR, delayBufferDataR, bufferLength, delayBufferLength, delayTimesArray[line], 1);

			//filterGenerator.lowPassFilter[0].process(dsp::ProcessContextReplacing<float>(block));
			//tu będzie jeszcze delayTimeDepentantFilter
			
			addDelayWithCurrentBuffer(leftChannel, bufferLength, delayBufferLength, dryBufferL, delayTimesNumber, amplitude / (float)numberDelayLines);
			addDelayWithCurrentBuffer(rightChannel, bufferLength, delayBufferLength, dryBufferR, delayTimesNumber, amplitude / (float)numberDelayLines);
		} 
		else
		{
			//copyBackToCurrentBuffer(buffer, leftChannel, bufferDataL, delayBufferDataL, bufferLength, delayBufferLength, delayTimesArray[line],  /*0.15 **/ mWetDry / (float)numberDelayLines);
			//filterGenerator.lowPassFilterLeft[line].process(dsp::ProcessContextReplacing<float>(block));
			//copyBackToCurrentBuffer(buffer, rightChannel, bufferDataR, delayBufferDataR, bufferLength, delayBufferLength, delayTimesArray[line] + spatialMaker.ITDCoefficients[line], /*0.15 **/ mWetDry / (float)numberDelayLines);
			//
			//reverbSize
			if (line <= reverbSize)
			{
				copyBackToCurrentBuffer(buffer, leftChannel, bufferDataL, delayBufferDataL, bufferLength, delayBufferLength, delayTimesArray[line], 1);
				filterGenerator.lowPassFilterLeft[line].process(dsp::ProcessContextReplacing<float>(block));
				copyBackToCurrentBuffer(buffer, rightChannel, bufferDataR, delayBufferDataR, bufferLength, delayBufferLength, delayTimesArray[line] + spatialMaker.ITDCoefficients[line], 1);
				
				
				dsp::AudioBlock<float> block(buffer);
				filterGenerator.lowPassFilterRight[line].process(dsp::ProcessContextReplacing<float>(block));
				addDelayWithCurrentBuffer(leftChannel, bufferLength, delayBufferLength, dryBufferL, delayTimesNumber, amplitude / (float)reverbSize);
				addDelayWithCurrentBuffer(rightChannel, bufferLength, delayBufferLength, dryBufferR, delayTimesNumber, amplitude / (float)reverbSize);
			}
			//addDelayWithCurrentBuffer(leftChannel, bufferLength, delayBufferLength, dryBufferL, delayTimesNumber, amplitude/** wet*/ / ((float)(delayTimesNumber)));
			//addDelayWithCurrentBuffer(rightChannel, bufferLength, delayBufferLength, dryBufferR, delayTimesNumber, (amplitude /*+ (float)spatialMaker.ILDCoefficients[line] * ILDwet*/) /** wet*/ / ((float)(delayTimesNumber)));
		} 
	}
	//filterGenerator.allPassFilter.process(dsp::ProcessContextReplacing<float>(block));

	bufferWritePosition += bufferLength; //przesuwamy pozycjê czyli miejsce do którego wklejamy nasz buffer
								//czyli je¿eli bufor ma 512 próbek to kolejmy wklejamy w miejsce od indeksu 513
	bufferWritePosition = bufferWritePosition % delayBufferLength;	//je¿eli pozycja wpisania wykracza poza d³ugoœæ bufora to znowu idzie na pocz¹tek:
															//np. bufor opóŸniaj¹cy to 2048. pozycja wpisania to 2048, dziêki modulo pozycja idzie znów na 
															//pocz¹tek czyli na 0, potem 1
															//mWritePosition to pole klasy - bêdziemy tego indeksu u¿ywaæ te¿ przy czytaniu z bufora

	//buffer.applyGain(2);
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
		delayBuffer.copyFrom(channel, bufferWritePosition, bufferData, bufferLength);
	}
	else
	{
		const int bufferRemaning = delayBufferLength - bufferWritePosition; //index 
		delayBuffer.copyFrom(channel, bufferWritePosition, bufferData, bufferRemaning);

		delayBuffer.copyFrom(channel, 0, bufferData, bufferLength - bufferRemaning);	//to co zosta³o wklejamy na pocz¹tek. 
																										//czyli to co zosta³o odciête (ca³y bufor to 
																										//bufferRemaining + ta reszta któr¹ tu doklejamy
	}
}

//
//void ReverbEngine::asd(AudioBuffer<float>& buffer, int channel, const float* bufferData, const float* delayBufferData,
//	const int bufferLength, const int delayBufferLength, int delayTime)
//{
//
//	const int bufferReadPosition = static_cast<int>(delayBufferLength + bufferWritePosition - (sampleRate_ * delayTime / 1000)) % delayBufferLength;
//
//	if (delayBufferLength > bufferLength + bufferReadPosition)
//	{
//		buffer.addFromWithRamp(channel, 0, delayBufferData + bufferReadPosition, bufferLength, 0.8/numberDelayLines, 0.8/numberDelayLines);
//	}
//	else																
//	{
//		const int bufferRemaining = delayBufferLength - bufferReadPosition;		
//		buffer.addFromWithRamp(channel, 0, delayBufferData + bufferReadPosition, bufferRemaining, 0.8 / numberDelayLines, 0.8 / numberDelayLines);
//		buffer.addFromWithRamp(channel, bufferRemaining, delayBufferData, bufferLength - bufferRemaining, 0.8/numberDelayLines, 0.8/numberDelayLines);
//	}
//}


void ReverbEngine::copyBackToCurrentBuffer(AudioBuffer<float>& buffer, int channel, const float* bufferData, const float* delayBufferData,
	const int bufferLength, const int delayBufferLength, int delayTime, float mWetDry)
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
		buffer.copyFrom(channel, 0, delayBufferData + bufferReadPosition, bufferLength, mWetDry);
	}
	else																//je¿eli nie mamy tyle próbek (bufor opóŸniaj¹cy siê 'koñczy') to bierzemy kawa³ek z koñca
																		//i resztê z pocz¹tku 
	{
		const int bufferRemaining = delayBufferLength - bufferReadPosition;		//ile wartoœci zosta³o w delay bufferze
		buffer.copyFrom(channel, 0, delayBufferData + bufferReadPosition, bufferRemaining, mWetDry);
		buffer.copyFrom(channel, bufferRemaining, delayBufferData, bufferLength - bufferRemaining, mWetDry);
	}
	//buffer.addFrom()
	//buffer.addFromWithRamp()
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

	//AudioBuffer<float>buffer_;
	//buffer_.setSize(2, bufferLength);
	//buffer_.clear();
	//buffer_.addFromWithRamp(channel, 0, bufferData, bufferLength, 1, 1);
	////buffer_.applyGain(0.1);
	//float* asd = buffer_.getWritePointer(channel);

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
	//delayBuffer.applyGain(0.9);
}