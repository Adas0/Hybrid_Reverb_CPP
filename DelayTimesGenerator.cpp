/*
  ==============================================================================

    DelayTimesGenerator.cpp
    Created: 16 Feb 2020 11:52:00am
    Author:  Adam

  ==============================================================================
*/

#include "DelayTimesGenerator.h"

int DelayTimesGenerator::generateRandomNumber()
{
	auto randomNumber = Random::getSystemRandom().nextInt(Range<int>(30, 300));
	return randomNumber;
}


std::vector<int> DelayTimesGenerator::getDelayTimes(int & delayTimesNumber)
{
	std::vector<int>delayTimes;
	delayTimes.clear();
	for (int i = 0; i < delayTimesNumber; ++i)
	{
		delayTimes.push_back(generateRandomNumber());
	}
	//std::sort(delayTimes.begin(), delayTimes.end());
	return delayTimes;
}

std::vector<float> DelayTimesGenerator::getNoiseCoefficients(int & numCoefficients)
{
	std::vector<float>noiseCoefficnets;
	noiseCoefficnets.clear();
	for (int i = 0; i < numCoefficients; ++i)
	{
		noiseCoefficnets.push_back(Random::getSystemRandom().nextFloat());
		//noiseCoefficnets[i] /= 10.0;
	}
	
	//std::sort(delayTimes.begin(), delayTimes.end());
	return noiseCoefficnets;
}



