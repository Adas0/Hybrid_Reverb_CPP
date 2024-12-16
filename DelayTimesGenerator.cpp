/*
  ==============================================================================

    DelayTimesGenerator.cpp
    Created: 16 Feb 2020 11:52:00am
    Author:  Adam Korytowski

  ==============================================================================
*/

#include "DelayTimesGenerator.h"

int DelayTimesGenerator::generateRandomNumber(int & lowTime, int& highTime)
{
	auto randomNumber = Random::getSystemRandom().nextInt(Range<int>(lowTime, highTime));
	return randomNumber;
}

std::vector<int> DelayTimesGenerator::getDelayTimes(int & delayTimesNumber, 
								int& lowTime, int& highTime, int& firstRefTime)
{
	std::vector<int>delayTimes;
	delayTimes.clear();

	for (int i = 0; i < delayTimesNumber/*/4*/; ++i)
	{ 
		delayTimes.push_back(this->delayTimesPrime_[i * 2]);
	}

	std::sort(delayTimes.begin(), delayTimes.end());
	std::reverse(delayTimes.begin(), delayTimes.end());

	return delayTimes; 
} 
 
