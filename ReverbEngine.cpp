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


	delayTimes.lowDelayTime = 80;
	delayTimes.highDelayTime = 800;
	delayTimes.firstReflectionTime = 80;
	delayTimesNumber = numberDelayLines;
	delayTimesArray = delayTimes.getDelayTimes(delayTimesNumber, delayTimes.lowDelayTime, delayTimes.highDelayTime, delayTimes.firstReflectionTime);
	//int lateTailArrayIndex = 2 * delayTimesNumber / 3;

	//lateReverb.lateReverbNumLines =  delayTimesNumber / 2; //biorę ostatnią 1/3 elementów tablicy zawierającej czasy opóźnienia
														  //i bede podmieniał je na odbicia większe niż zakres czasów opóźnienia zwykłych odbić
	//lateReverb.addLateReverb(delayTimesArray);
	 
	delayTimesArray[delayTimesArray.size() - 1] = 0;

}


void ReverbEngine::process(AudioBuffer<float>&buffer)
{
	dsp::AudioBlock<float> block(buffer);
	//buffer.applyGain(0.1);
	const int bufferLength = buffer.getNumSamples();
	const int delayBufferLength = delayBuffer.getNumSamples();

	AudioBuffer<float>dry = buffer;
	const float* delayBufferDataL = delayBuffer.getReadPointer(leftChannel);
	const float* delayBufferDataR = delayBuffer.getReadPointer(rightChannel);

	const float* bufferDataLa = buffer.getReadPointer(leftChannel);
	const float* bufferDataRa = buffer.getReadPointer(rightChannel);

	for (int channel = 0; channel < numInputChannels; ++channel)
	{
		const float* bufferData = buffer.getReadPointer(channel);
		const float* delayBufferData = delayBuffer.getReadPointer(channel);
		
		copyBufferToDelayBuffer(channel, bufferData, delayBufferData, bufferLength, delayBufferLength);

		buffer.applyGainRamp(channel, 0, bufferLength, 0.9, 0.9);
	}

	/*float* dryBufferL = buffer.getWritePointer(leftChannel);
	float* dryBufferR = buffer.getWritePointer(rightChannel);*/

	const float* bufferDataL = buffer.getReadPointer(leftChannel);
	const float* bufferDataR = buffer.getReadPointer(rightChannel);

	for (int line = 0; line < delayTimesNumber; ++line)
	{

		float amplitude = 0.999;
		
		if (line == delayTimesNumber - 1)	//direct sound
		{  
			////wetDry
			//copyBackToCurrentBuffer(buffer, leftChannel, bufferDataL, delayBufferDataL, bufferLength, delayBufferLength, delayTimesArray[line],/* mWetDry*/ /*1 - */(1 - mWetDry)/* / (float)numberDelayLines*/) /** numberDelayLines*/;
			//copyBackToCurrentBuffer(buffer, rightChannel, bufferDataR, delayBufferDataR, bufferLength, delayBufferLength, delayTimesArray[line],/* mWetDry*/ /*1 -*/ (1 - mWetDry) /*/ (float)numberDelayLines*/) /** numberDelayLines*/;
			copyBackToCurrentBuffer(buffer, leftChannel, bufferDataL, delayBufferDataL, bufferLength, delayBufferLength, delayTimesArray[line], 1);
			copyBackToCurrentBuffer(buffer, rightChannel, bufferDataR, delayBufferDataR, bufferLength, delayBufferLength, delayTimesArray[line], 1);

			//filterGenerator.lowPassFilter[0].process(dsp::ProcessContextReplacing<float>(block));
			//tu będzie jeszcze delayTimeDepentantFilter
			
			addDelayWithCurrentBuffer(leftChannel, bufferLength, delayBufferLength, bufferDataL, delayTimesNumber, (1 - amplitude) / (float)numberDelayLines, dry);
			addDelayWithCurrentBuffer(rightChannel, bufferLength, delayBufferLength, bufferDataR, delayTimesNumber, (1 - amplitude) / (float)numberDelayLines, dry);
		}

		else
		{
			if (line >= delayTimesNumber - reverbSize)
			{
				copyBackToCurrentBuffer(buffer, leftChannel, bufferDataLa, delayBufferDataL, bufferLength, delayBufferLength, delayTimesArray[line], 1);
				filterGenerator.lowPassFilterLeft[line].process(dsp::ProcessContextReplacing<float>(block));
				copyBackToCurrentBuffer(buffer, rightChannel, bufferDataRa, delayBufferDataR, bufferLength, delayBufferLength, delayTimesArray[line] + spatialMaker.ITDCoefficients[line], 1);


				dsp::AudioBlock<float> block(buffer);
				filterGenerator.lowPassFilterRight[line].process(dsp::ProcessContextReplacing<float>(block));


				addDelayWithCurrentBuffer(leftChannel, bufferLength, delayBufferLength, bufferDataLa, NULL, mWetDry / (float)numberDelayLines, dry);
				addDelayWithCurrentBuffer(rightChannel, bufferLength, delayBufferLength, bufferDataRa, NULL, mWetDry / (float)numberDelayLines, dry);
			}
			//AudioBuffer<float>_buffer = buffer;
			//if (delayBufferLength > bufferLength + bufferWritePosition)
			//{
			//	buffer.addFrom(0, bufferWritePosition, dry, bufferLength, 0.9, 0.9);
			//	buffer.addFrom(1, bufferWritePosition, dry, bufferLength, 0.9, 0.9);
			//}
			//else
			//{
			//	buffer.addFrom(0, bufferWritePosition, dry, delayBufferLength - bufferWritePosition, 0.9, 0.9);
			//	buffer.addFrom(1, bufferWritePosition, dry, delayBufferLength - bufferWritePosition, 0.9, 0.9);

			//	buffer.addFrom(0, 0, dry + delayBufferLength - bufferWritePosition, bufferLength - delayBufferLength + bufferWritePosition, 0.9, 0.9);
			//	buffer.addFrom(1, 0, dry + delayBufferLength - bufferWritePosition, bufferLength - delayBufferLength + bufferWritePosition, 0.9, 0.9);
			//}
		}

	}
	//filterGenerator.allPassFilter.process(dsp::ProcessContextReplacing<float>(block));

	bufferWritePosition += bufferLength;

	bufferWritePosition = bufferWritePosition % delayBufferLength;	


}


void ReverbEngine::copyBufferToDelayBuffer(int channel, const float* bufferData, const float* delayBufferData,
	const int bufferLength, const int delayBufferLength)
{

	if (delayBufferLength > bufferLength + bufferWritePosition)	 
	{
		delayBuffer.copyFromWithRamp(channel, bufferWritePosition, bufferData, bufferLength, 0.9, 0.9);
	}
	else
	{
		delayBuffer.copyFromWithRamp(channel, bufferWritePosition, bufferData, delayBufferLength - bufferWritePosition, 0.9, 0.9);

		delayBuffer.copyFromWithRamp(channel, 0, bufferData + delayBufferLength - bufferWritePosition, bufferLength - delayBufferLength + bufferWritePosition, 0.9, 0.9);
	}
}


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
	const int delayBufferLength, const float* bufferData, int delayTimesNumber, float amplitudeMultiplier, AudioBuffer<float>buffer_)
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

void ReverbEngine::addDelayWithCurrentBuffer2(int channel, const int bufferLength,
	const int delayBufferLength, const float* bufferData, int delayTimesNumber, float amplitudeMultiplier, AudioBuffer<float>buffer_)
{


	if (delayBufferLength > bufferLength + bufferWritePosition)
	{

		delayBuffer.addFrom(channel, bufferWritePosition, buffer_, bufferLength, amplitudeMultiplier, amplitudeMultiplier);		//ostatnie 2 argumenty - jak szybko zanika 
																														// - mno¿enie amplitudy z ka¿dym odbiciem

	}
	else
	{
		const int bufferRemaining = delayBufferLength - bufferWritePosition;
		delayBuffer.addFrom(channel, bufferWritePosition, buffer_, bufferRemaining, amplitudeMultiplier, amplitudeMultiplier);
		delayBuffer.addFrom(channel, 0, buffer_, bufferLength - bufferRemaining, amplitudeMultiplier, amplitudeMultiplier);

	}
	//mDelayBuffer.applyGain(0.7);
	//delayBuffer.applyGain(0.9);
}