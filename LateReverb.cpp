/*
  ==============================================================================

    LateReverb.cpp
    Created: 29 Mar 2020 11:22:16am
    Author:  Adam Korytowski

  ==============================================================================
*/

#include "LateReverb.h" 

void LateReverb::addLateReverb(std::vector<int>& delayTimes)
{
	for (int line = 0; line < this->lateReverbNumLines - 1; ++line)
	{
																
		delayTimes [line + (numberDelayLines - this->lateReverbNumLines) - 1] = this->highDelayTime + Random::getSystemRandom().nextInt(Range<int>(0, 50));  
	}
}

AudioBuffer<float> LateReverb::createNoiseBufferArray(int& bufferSize)
{	
	AudioBuffer<float>noiseB;
	noiseB.setSize(2, bufferSize);
	noiseB.clear();
	for (int sample = 0; sample < bufferSize; ++sample)
	{
		noiseB.addSample(0, sample, Random::getSystemRandom().nextFloat() );
		noiseB.addSample(1, sample, Random::getSystemRandom().nextFloat() );

	}
	return noiseB;
}

float LateReverb::generateRandomNoise()
{
	float level = Random::getSystemRandom().nextFloat() / 10;
	return level;
}
