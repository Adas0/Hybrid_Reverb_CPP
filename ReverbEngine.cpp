﻿/*
  ==============================================================================

    ReverbEngine.cpp
    Created: 28 Mar 2020 12:27:48pm
    Author:  Adam Korytowski

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
	delayTimes.firstReflectionTime = 100;
	delayTimesNumber = numberDelayLines;
	delayTimesArray.clear();
	delayTimesArray = delayTimes.getDelayTimes(delayTimesNumber, delayTimes.lowDelayTime, delayTimes.highDelayTime, delayTimes.firstReflectionTime);
	
	delayTimesArray[numberDelayLines - 1] = 0;
	delayTimesArray[delayTimesArray.size() - 2] = 0;

	for (int i = 0; i < numberDelayLines; ++i)
	{
		noiseBuffers[i] = (lateReverb.createNoiseBufferArray(samplesPerBlock));

	}
}


void ReverbEngine::process(AudioBuffer<float>&buffer)
{

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
			if (delayTimesArray[line] < reverbSize)
			{
				if (line == numberDelayLines - 2)
					copyBackToCurrentBuffer(buffer, leftChannel, bufferDataLa, delayBufferDataL,
								bufferLength, delayBufferLength, delayTimesArray[line] + firstRefTime + 10);
				else 
					copyBackToCurrentBuffer(buffer, leftChannel, bufferDataLa, delayBufferDataL,
								bufferLength, delayBufferLength, delayTimesArray[line] + firstRefTime);

					
					if (ITD_on)
						copyBackToCurrentBuffer(buffer, rightChannel, bufferDataRa, delayBufferDataR, bufferLength, delayBufferLength,
													delayTimesArray[line] + spatialMaker.ITDCoefficients[line] + firstRefTime);
					else
						copyBackToCurrentBuffer(buffer, rightChannel, bufferDataRa, delayBufferDataR, bufferLength, delayBufferLength,
							delayTimesArray[line] + firstRefTime);


					dsp::AudioBlock<float> block(buffer);
					if (filtersOption == 1)
						filterGenerator.lowPassFilterRight[line].process(dsp::ProcessContextReplacing<float>(block));
					else
						filterGenerator.constantFiltersRight[line].process(dsp::ProcessContextReplacing<float>(block));


					float* noiseBufferDataL = noiseBuffer.getWritePointer(0);
					float* noiseBufferDataR = noiseBuffer.getWritePointer(1);
					

					noiseBuffer.setSize(2, bufferLength);
					noiseBuffer.clear();
					for (int sample = 0; sample < bufferLength; ++sample)
					{
						noiseBuffer.addSample(leftChannel, sample, Random::getSystemRandom().nextFloat() * 2);
						noiseBuffer.addSample(rightChannel, sample, Random::getSystemRandom().nextFloat() * 2);

					}

					if (noiseOn)
					{
						dsp::AudioBlock<float>noiseBlock(noiseBuffer);
								filterGenerator.noiseFilters[line].process(dsp::
											ProcessContextReplacing<float>(noiseBlock));

						for (int sample = 0; sample < bufferLength; ++sample)
						{
							bufferWriteL[sample] *= (noiseBufferDataL[sample]);
							bufferWriteR[sample] *= (noiseBufferDataR[sample]);

						}
					}
						
					if (line == numberDelayLines - 2)
					{
						
							addDelayWithCurrentBuffer(leftChannel, bufferLength, delayBufferLength, bufferDataLa,
															mWetDry  / (float)numberDelayLines);
						
							addDelayWithCurrentBuffer(rightChannel, bufferLength, delayBufferLength, bufferDataRa, 
															(mWetDry  - lateralAmplitudeDifference) / (float)numberDelayLines);

					}
					else
					{
						if (ILD_on)
							addDelayWithCurrentBuffer(leftChannel, bufferLength, delayBufferLength,
												bufferDataLa, (mWetDry + 14 * spatialMaker.ILDCoefficients[line]) / (float)numberDelayLines);
						else
							addDelayWithCurrentBuffer(leftChannel, bufferLength, delayBufferLength,
												bufferDataLa, mWetDry / (float)numberDelayLines);

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
