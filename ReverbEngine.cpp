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
	 

	//delayTimesArray[delayTimesArray.size() - 1] = 0;

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
	//buffer.applyGain(0.1);
	const int bufferLength = buffer.getNumSamples();
	const int delayBufferLength = delayBuffer.getNumSamples();

	
	const float* delayBufferDataL = delayBuffer.getReadPointer(leftChannel);
	const float* delayBufferDataR = delayBuffer.getReadPointer(rightChannel);


	


	/*AudioBuffer<float>asd;
	asd.setSize(2, bufferLength);
	asd.clear();
	asd.addFrom(0, 0, dryBufferL, bufferLength, 1);*/
	/*AudioBuffer<float>asd;
	asd.setSize(2, bufferLength);
	asd.clear();
	asd.addFrom(0, 0, dryBufferL, bufferLength, 1);*/
	//buffer.applyGain(0.9);
	for (int channel = 0; channel < numInputChannels; ++channel)
	{
		const float* bufferData = buffer.getReadPointer(channel);
		const float* delayBufferData = delayBuffer.getReadPointer(channel);
		
		copyBufferToDelayBuffer(channel, bufferData, delayBufferData, bufferLength, delayBufferLength);

		//buffer.applyGainRamp(channel, 0, bufferLength, 1.0 - (mWetDry /** numberDelayLines*/), 1.0 - (mWetDry /** numberDelayLines*/));
		buffer.applyGainRamp(channel, 0, bufferLength, 0.9, 0.9);
	}


	//buffer.applyGain(0.05);
	float* dryBufferL = buffer.getWritePointer(leftChannel);
	float* dryBufferR = buffer.getWritePointer(rightChannel);

	const float* bufferDataL = buffer.getReadPointer(leftChannel);
	const float* bufferDataR = buffer.getReadPointer(rightChannel);
	 
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
	//	copyBackToCurrentBuffer(buffer, rightChannel, bufferDataR, delayBufferDataR, bufferLength, delayBufferLength, i);
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
	

	/*for (int line = 0; line < delayTimesNumber; ++line)
	{*/
		/*if (filter < delayTimesNumber - lateReverb.lateReverbNumLines)
		{*/  
		//float delayTimeDependantAmp = (1 - (delayTimesArray[line] /*/ delayTimes.highDelayTime) *//  delayTimes.delayTimesPrime[delayTimesNumber + 60]) * 0.7 );
		//float delayTimeDependantAmp = (1 - ((line * 4) * 0.85 / 100)) * 1.009; 
		//float amplitude = 0.5/numberDelayLines;
		/*delayTimesArray[0] = 500;
		delayTimesArray[1] = 50;*/
		//float asd = std::pow(0.05 / (float)delayTimesArray[line], 1 / (float)delayTimesArray[line]);
		//asd = 1.7 - asd;
		//float asd = std::pow(0.05 / 500, 1/500);
		//float delayTimeDependantAmp = asd/delayTimesNumber;
		//float delayTimeDependantAmp = 0.7;
		//float wet = 1 - wetDry;
		//float delayTimeDependantAmp = 0.4f; 
		
		//if (line == delayTimesNumber - 1)	//direct sound
		//{  
		//	////wetDry
		//	//copyBackToCurrentBuffer(buffer, leftChannel, bufferDataL, delayBufferDataL, bufferLength, delayBufferLength, delayTimesArray[line],/* mWetDry*/ /*1 - */(1 - mWetDry)/* / (float)numberDelayLines*/) /** numberDelayLines*/;
		//	//copyBackToCurrentBuffer(buffer, rightChannel, bufferDataR, delayBufferDataR, bufferLength, delayBufferLength, delayTimesArray[line],/* mWetDry*/ /*1 -*/ (1 - mWetDry) /*/ (float)numberDelayLines*/) /** numberDelayLines*/;
		//	copyBackToCurrentBuffer(buffer, leftChannel, bufferDataL, delayBufferDataL, bufferLength, delayBufferLength, delayTimesArray[line], 0.02);
		//	copyBackToCurrentBuffer(buffer, rightChannel, bufferDataR, delayBufferDataR, bufferLength, delayBufferLength, delayTimesArray[line], 0.02);

		//	//filterGenerator.lowPassFilter[0].process(dsp::ProcessContextReplacing<float>(block));
		//	//tu będzie jeszcze delayTimeDepentantFilter
		//	
		//	/*addDelayWithCurrentBuffer(leftChannel, bufferLength, delayBufferLength, dryBufferL, delayTimesNumber, amplitude / (float)numberDelayLines);
		//	addDelayWithCurrentBuffer(rightChannel, bufferLength, delayBufferLength, dryBufferR, delayTimesNumber, amplitude / (float)numberDelayLines);
		//*/} 

		//else
		//{
			//copyBackToCurrentBuffer(buffer, leftChannel, bufferDataL, delayBufferDataL, bufferLength, delayBufferLength, delayTimesArray[line],  /*0.15 **/ mWetDry / (float)numberDelayLines);
			//filterGenerator.lowPassFilterLeft[line].process(dsp::ProcessContextReplacing<float>(block));
			//copyBackToCurrentBuffer(buffer, rightChannel, bufferDataR, delayBufferDataR, bufferLength, delayBufferLength, delayTimesArray[line] + spatialMaker.ITDCoefficients[line], /*0.15 **/ mWetDry / (float)numberDelayLines);
			//
			//reverbSize
			/*if (line <= reverbSize)
			{*/

			/*copyBackToCurrentBuffer(buffer, leftChannel, bufferDataL, delayBufferDataL, bufferLength, delayBufferLength, 0, 1);
			copyBackToCurrentBuffer(buffer, rightChannel, bufferDataR, delayBufferDataR, bufferLength, delayBufferLength, 0, 1);

			addDelayWithCurrentBuffer(leftChannel, bufferLength, delayBufferLength, dryBufferL, delayTimesNumber, 1);
			addDelayWithCurrentBuffer(rightChannel, bufferLength, delayBufferLength, dryBufferR, delayTimesNumber, 1);*/


				
			
				copyBackToCurrentBuffer(buffer, leftChannel, bufferDataL, delayBufferDataL, bufferLength, delayBufferLength, 300, 1);
				//filterGenerator.lowPassFilterLeft[line].process(dsp::ProcessContextReplacing<float>(block));
				copyBackToCurrentBuffer(buffer, rightChannel, bufferDataR, delayBufferDataR, bufferLength, delayBufferLength, 300 /*+ spatialMaker.ITDCoefficients[line]*/, 1);
				
				
				//dsp::AudioBlock<float> block(buffer);
				//filterGenerator.lowPassFilterRight[line].process(dsp::ProcessContextReplacing<float>(block));
				
				//asd.applyGain(1 - mWetDry);

				addDelayWithCurrentBuffer(leftChannel, bufferLength, delayBufferLength, bufferDataL, NULL, 1);
				addDelayWithCurrentBuffer(rightChannel, bufferLength, delayBufferLength, bufferDataR, NULL, 1);


			//	copyBackToCurrentBuffer(buffer, leftChannel, bufferDataL, delayBufferDataL, bufferLength, delayBufferLength, 0, 0.3);
			//	//filterGenerator.lowPassFilterLeft[line].process(dsp::ProcessContextReplacing<float>(block));
			//	copyBackToCurrentBuffer(buffer, rightChannel, bufferDataR, delayBufferDataR, bufferLength, delayBufferLength, 0 /*+ spatialMaker.ITDCoefficients[line]*/, 0.3);


			//	//dsp::AudioBlock<float> block(buffer);
			//	//filterGenerator.lowPassFilterRight[line].process(dsp::ProcessContextReplacing<float>(block));

			//	//asd.applyGain(1 - mWetDry);

			//	addDelayWithCurrentBuffer(leftChannel, bufferLength, delayBufferLength, dryBufferL, NULL, 0.3);
			//	addDelayWithCurrentBuffer(rightChannel, bufferLength, delayBufferLength, dryBufferR, NULL, 0.3);
			////}
			//addDelayWithCurrentBuffer(leftChannel, bufferLength, delayBufferLength, dryBufferL, delayTimesNumber, amplitude/** wet*/ / ((float)(delayTimesNumber)));
			//addDelayWithCurrentBuffer(rightChannel, bufferLength, delayBufferLength, dryBufferR, delayTimesNumber, (amplitude /*+ (float)spatialMaker.ILDCoefficients[line] * ILDwet*/) /** wet*/ / ((float)(delayTimesNumber)));
		//} 
	//}
	//filterGenerator.allPassFilter.process(dsp::ProcessContextReplacing<float>(block));

	bufferWritePosition += bufferLength; //przesuwamy pozycjê czyli miejsce do którego wklejamy nasz buffer
								//czyli je¿eli bufor ma 512 próbek to kolejmy wklejamy w miejsce od indeksu 513
	bufferWritePosition = bufferWritePosition % delayBufferLength;	//je¿eli pozycja wpisania wykracza poza d³ugoœæ bufora to znowu idzie na pocz¹tek:
															//np. bufor opóŸniaj¹cy to 2048. pozycja wpisania to 2048, dziêki modulo pozycja idzie znów na 
															//pocz¹tek czyli na 0, potem 1
															//mWritePosition to pole klasy - bêdziemy tego indeksu u¿ywaæ te¿ przy czytaniu z bufora



	/*buffer.addFrom(channel, 0, delayBufferData - delayTime + bufferLength, bufferLength, );
	buffer.addFrom()*/
	//buffer.applyGain(10);

	//delayBufferData to pointer, który wskazuje na pierwszy element 
	//buffer.applyGain(2);
}


void ReverbEngine::copyBufferToDelayBuffer(int channel, const float* bufferData, const float* delayBufferData,
	const int bufferLength, const int delayBufferLength)
{

	if (delayBufferLength > bufferLength + bufferWritePosition)	 
	{
		delayBuffer.copyFromWithRamp(channel, bufferWritePosition, bufferData, bufferLength, 0.1, 0.1);
	}
	else
	{
		delayBuffer.copyFromWithRamp(channel, bufferWritePosition, bufferData, delayBufferLength - bufferWritePosition, 0.1, 0.1);

		delayBuffer.copyFromWithRamp(channel, 0, bufferData + delayBufferLength - bufferWritePosition, bufferLength - delayBufferLength + bufferWritePosition, 0.1, 0.1);
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



	
	if (delayBufferLength > bufferLength + bufferReadPosition)			
	{
		buffer.copyFrom(channel, 0, delayBufferData + bufferReadPosition, bufferLength, mWetDry);
		//delayBufferData + bufferReadPosition			- delayBufferData to wskaźnik na pierwszy element delayBuffer. Dodając do tego 
														//bufferReadPosition przesuwamy po prostu ten wskaźnik o wartość bufferReadPosition próbek.
														//właściwie bufferReadPosition jest const więc się nie przesuwa, a u nas bufferReadPosition
														//przesuwa się coraz dalej, bo jest zależny od bufferWritePosition
	}			
	else																
	{
		buffer.copyFrom(channel, 0, delayBufferData + bufferReadPosition, delayBufferLength - bufferReadPosition, mWetDry); //w to samo miejsce, tylko mniejszy kawałek
		buffer.copyFrom(channel, delayBufferLength - bufferReadPosition, delayBufferData, bufferLength - delayBufferLength + bufferReadPosition, mWetDry);
		//delayBufferLength - bufferReadPosition		- tyle próbek zostało, więc od tego indeksu będziemy wklejać dalszą, pozostałą część bufora
	}
	//buffer.addFrom()
	//buffer.addFromWithRamp()
	//buffer.applyGain(0.9);
}

void ReverbEngine::addDelayWithCurrentBuffer(int channel, const int bufferLength,
	const int delayBufferLength, const float* bufferData, int delayTimesNumber, float amplitudeMultiplier)
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