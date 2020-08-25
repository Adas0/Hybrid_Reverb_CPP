/*
  ==============================================================================

    LateReverb.cpp
    Created: 29 Mar 2020 11:22:16am
    Author:  Adam

  ==============================================================================
*/

#include "LateReverb.h" 


void LateReverb::addLateReverb(std::vector<int>& delayTimes)
{
	for (int line = 0; line < this->lateReverbNumLines - 1; ++line)
	{
		//delayTimes [line + (numberDelayLines - this->lateReverbNumLines) - 1] = this->highDelayTime + primeNumbers[Random::getSystemRandom().nextInt(Range<int>(0, primeNumbers.size()-5))] * line;
																		//liczba pierwsza, żeby nie było interferencji
		delayTimes [line + (numberDelayLines - this->lateReverbNumLines) - 1] = this->highDelayTime 
																				+ Random::getSystemRandom().nextInt(Range<int>(0, 50));  //czyli np. 310, 320, 330
	}
}

AudioBuffer<float> LateReverb::createNoiseBufferArray(int& bufferSize)
{
	//std::vector<AudioBuffer<float>> noiseBuffers;
	//noiseBuffers.clear();

	//noiseBuffer.setSize(2, bufferSize);
	//for (int line = 0; line < numberDelayLines; ++line)
	//{
	//	noiseBuffer.clear();
	//	for (int sample = 0; sample < bufferSize; ++sample)
	//	{
	//		//noiseBuffer.clear();
	//		noiseBuffer.addSample(0, sample,  1.2 * Random::getSystemRandom().nextFloat() / 1.0f);
	//		noiseBuffer.addSample(1, sample, 1.2 * Random::getSystemRandom().nextFloat() / 1.0f);
	//	}

	//	noiseBuffers.push_back(noiseBuffer);
	//}
	
	AudioBuffer<float>noiseB;
	noiseB.setSize(2, bufferSize);
	noiseB.clear();
	for (int sample = 0; sample < bufferSize; ++sample)
	{
		noiseB.addSample(0, sample, Random::getSystemRandom().nextFloat() );
		noiseB.addSample(1, sample, Random::getSystemRandom().nextFloat() );

	}
	/*noiseBuffer.setSize(2, bufferLength);
					noiseBuffer.clear();
					for (int sample = 0; sample < bufferLength; ++sample)
					{
						noiseBuffer.addSample(leftChannel, sample,  Random::getSystemRandom().nextFloat());
						noiseBuffer.addSample(rightChannel, sample, Random::getSystemRandom().nextFloat());

					}*/
	
	/*for (int buffer = 0; buffer < numberDelayLines; ++buffer)
	{
		noiseBuffers.push_back(noiseBuffer);
	}*/
	return noiseB;
}


void LateReverb::addLateNoise(AudioBuffer<float>noiseBuffer, AudioBuffer<float>&buffer, float* noiseBufferDataL, float* noiseBufferDataR,
																float* bufferWriteL, float* bufferWriteR)
{


}


float LateReverb::generateRandomNoise()
{
	float level = Random::getSystemRandom().nextFloat() / 10;
	return level;
}