/*
  ==============================================================================

    DelayTimesGenerator.cpp
    Created: 16 Feb 2020 11:52:00am
    Author:  Adam

  ==============================================================================
*/

#include "DelayTimesGenerator.h"

int DelayTimesGenerator::generateRandomNumber(int & lowTime, int& highTime)
{
	auto randomNumber = Random::getSystemRandom().nextInt(Range<int>(lowTime, highTime));
	return randomNumber;
}


std::vector<int> DelayTimesGenerator::getDelayTimes(int & delayTimesNumber, int& lowTime, int& highTime)
{
	std::vector<int>delayTimes;
	delayTimes.clear();
	for (int i = 0; i < delayTimesNumber; ++i)
	{ 
		delayTimes.push_back(this->delayTimesPrime[2 * i + 6]/*generateRandomNumber(lowTime, highTime)*/);
	}
	std::sort(delayTimes.begin(), delayTimes.end());
	std::reverse(delayTimes.begin(), delayTimes.end());
	return delayTimes; 
} 
 