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

	/*...*/

	const int delayBufferSize = 4 * (sampleRate);
	delayBuffer.setSize(numChannels, delayBufferSize);
	delayBuffer.clear();


	delayTimes.lowDelayTime = 400;
	delayTimes.highDelayTime = 1200;
	//delayTimes.highDelayTime = DelayTimesGenerator::delayTimesPrime_[3 * 160] * 2 + 50;
	delayTimes.firstReflectionTime = 300;
	delayTimesNumber = numberDelayLines;
	delayTimesArray.clear();
	delayTimesArray = delayTimes.getDelayTimes(delayTimesNumber, delayTimes.lowDelayTime, delayTimes.highDelayTime, delayTimes.firstReflectionTime);
	//int lateTailArrayIndex = 2 * delayTimesNumber / 3;

	//lateReverb.lateReverbNumLines =  delayTimesNumber / 2; //biorę ostatnią 1/3 elementów tablicy zawierającej czasy opóźnienia
														  //i bede podmieniał je na odbicia większe niż zakres czasów opóźnienia zwykłych odbić
	//lateReverb.addLateReverb(delayTimesArray);
	
	delayTimesArray[numberDelayLines - 1] = 0;
	delayTimesArray[delayTimesArray.size() - 2] = 0;

	noiseArray = lateReverb.createNoiseBufferArray(samplesPerBlock);
	
	

	//noiseVector.clear();
	for (int sample = 0; sample < samplesPerBlock; ++sample)
	{
		//asdf.clear();
		noiseVector.push_back(Random::getSystemRandom().nextFloat());
	}
	
	
}


void ReverbEngine::process(AudioBuffer<float>&buffer)
{
	//dsp::AudioBlock<float> block(buffer);
	//buffer.applyGain(0.1);

	const int bufferLength = buffer.getNumSamples();
	const int delayBufferLength = delayBuffer.getNumSamples();

	const float* delayBufferDataL = delayBuffer.getReadPointer(leftChannel);
	const float* delayBufferDataR = delayBuffer.getReadPointer(rightChannel);

	const float* bufferDataLa = buffer.getReadPointer(leftChannel);
	const float* bufferDataRa = buffer.getReadPointer(rightChannel);

	/*...*/

	for (int channel = 0; channel < numInputChannels; ++channel)
	{
		const float* bufferData = buffer.getReadPointer(channel);
		const float* delayBufferData = delayBuffer.getReadPointer(channel);
		
		copyBufferToDelayBuffer(channel, bufferData, delayBufferData, bufferLength, delayBufferLength);

	}

	const float* bufferDataL = buffer.getReadPointer(leftChannel);
	const float* bufferDataR = buffer.getReadPointer(rightChannel);

	for (int line = 0; line < delayTimesNumber; ++line)
	{
		float* bufferWriteL = buffer.getWritePointer(leftChannel);
		float* bufferWriteR = buffer.getWritePointer(rightChannel);
		
		if (line == numberDelayLines - 1)	//direct sound
		{  
			copyBackToCurrentBuffer(buffer, leftChannel, bufferDataL, delayBufferDataL, bufferLength, 
															delayBufferLength, delayTimesArray[line]);

			copyBackToCurrentBuffer(buffer, rightChannel, bufferDataR, delayBufferDataR, bufferLength, 
															delayBufferLength, delayTimesArray[line]);

			
			addDelayWithCurrentBuffer(leftChannel, bufferLength, delayBufferLength, bufferDataL, 
															(1 - mWetDry) / (float)numberDelayLines);
			addDelayWithCurrentBuffer(rightChannel, bufferLength, delayBufferLength, bufferDataR, 
															(1 - mWetDry) / (float)numberDelayLines);

		}

		else
		{
			if (line >= delayTimesNumber - reverbSize)
			{

					copyBackToCurrentBuffer(buffer, leftChannel, bufferDataLa, delayBufferDataL, 
								bufferLength, delayBufferLength,delayTimesArray[line] + firstRefTime);

					/*dsp::AudioBlock<float> blockLeftChannel(buffer);
					filterGenerator.lowPassFilterLeft[line].process(dsp::
											ProcessContextReplacing<float>(blockLeftChannel));*/
					

					copyBackToCurrentBuffer(buffer, rightChannel, bufferDataRa, delayBufferDataR, bufferLength, delayBufferLength,
													delayTimesArray[line] + spatialMaker.ITDCoefficients[line] + firstRefTime);


					dsp::AudioBlock<float> block(buffer);
					if (filtersOption == 1)
						filterGenerator.lowPassFilterRight[line].process(dsp::ProcessContextReplacing<float>(block));
					else
						filterGenerator.constantFiltersRight[line].process(dsp::ProcessContextReplacing<float>(block));


					noiseBuffer.setSize(2, bufferLength);
					noiseBuffer.clear();
					for (int sample = 0; sample < bufferLength; ++sample)
					{
						noiseBuffer.addSample(leftChannel, sample, noiseIntensity * Random::getSystemRandom().nextFloat());
						noiseBuffer.addSample(rightChannel, sample, noiseIntensity * Random::getSystemRandom().nextFloat());

					}

					dsp::AudioBlock<float>noiseBlock(noiseBuffer);
					filterGenerator.noiseFilters[line].process(dsp::ProcessContextReplacing<float>(noiseBlock));
					

					float* noiseBufferDataL = noiseBuffer.getWritePointer(0);
					float* noiseBufferDataR = noiseBuffer.getWritePointer(1);
					
					for (int sample = 0; sample < bufferLength; ++sample)
					{
							bufferWriteL[sample] *= noiseBufferDataL[sample];
							bufferWriteR[sample] *= noiseBufferDataR[sample];
						
					}

					if (line == numberDelayLines - 2)
					{
						addDelayWithCurrentBuffer(leftChannel, bufferLength, delayBufferLength, bufferDataLa,
															mWetDry / (float)numberDelayLines);
						addDelayWithCurrentBuffer(rightChannel, bufferLength, delayBufferLength, bufferDataRa, 
															(mWetDry - lateralAmplitudeDifference) / (float)numberDelayLines);

					}
					else
					{
						addDelayWithCurrentBuffer(leftChannel, bufferLength, delayBufferLength, 
												bufferDataLa, mWetDry  / (float)numberDelayLines);
						addDelayWithCurrentBuffer(rightChannel, bufferLength, delayBufferLength, 
												bufferDataRa, mWetDry / (float)numberDelayLines);
					}		
			}
		}
	}

	bufferWritePosition += bufferLength;

	bufferWritePosition = bufferWritePosition % delayBufferLength;	


}


void ReverbEngine::copyBufferToDelayBuffer(int channel, const float* bufferData, 
				const float* delayBufferData, const int bufferLength, const int delayBufferLength)
{

	if (delayBufferLength > bufferLength + bufferWritePosition)	 
	{
		delayBuffer.copyFrom(channel, bufferWritePosition, bufferData, bufferLength);
	}
	else
	{
		delayBuffer.copyFrom(channel, bufferWritePosition, bufferData, 
											delayBufferLength - bufferWritePosition);

		delayBuffer.copyFrom(channel, 0, bufferData + delayBufferLength - bufferWritePosition, 
											bufferLength - delayBufferLength + bufferWritePosition);
	}
}


void ReverbEngine::copyBackToCurrentBuffer(AudioBuffer<float>& buffer, int channel, 
			const float* bufferData, const float* delayBufferData, const int bufferLength, 
												const int delayBufferLength, int delayTime)
{

	const int bufferReadPosition = int(delayBufferLength + bufferWritePosition - 
									(sampleRate_ * delayTime / 1000)) % delayBufferLength;

	if (delayBufferLength > bufferLength + bufferReadPosition)			
	{
		buffer.copyFrom(channel, 0, delayBufferData + bufferReadPosition, bufferLength);
	}			
	else																
	{
		buffer.copyFrom(channel, 0, delayBufferData + bufferReadPosition, 
									delayBufferLength - bufferReadPosition);

		buffer.copyFrom(channel, delayBufferLength - bufferReadPosition, delayBufferData, 
									bufferLength - delayBufferLength + bufferReadPosition);

	}
}

void ReverbEngine::addDelayWithCurrentBuffer(int channel, const int bufferLength,
			const int delayBufferLength, const float* bufferData, float amplitudeMultiplier)
{ 
	if (delayBufferLength > bufferLength + bufferWritePosition)
	{

		delayBuffer.addFromWithRamp(channel, bufferWritePosition, bufferData, 
						bufferLength, amplitudeMultiplier, amplitudeMultiplier);		
																														
	}

	/*...*/

	else 
	{
		delayBuffer.addFromWithRamp(channel, bufferWritePosition, bufferData, 
				delayBufferLength - bufferWritePosition, amplitudeMultiplier, amplitudeMultiplier);

		delayBuffer.addFromWithRamp(channel, 0, bufferData, bufferLength - 
				delayBufferLength + bufferWritePosition, amplitudeMultiplier, amplitudeMultiplier);

	}
}
