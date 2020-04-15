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
	for (int i = 0; i < delayTimesNumber/2; ++i)
	{ 
		delayTimes.push_back(this->delayTimesPrime_[3 * i] /*+ 300*//*generateRandomNumber(lowTime, highTime)*/);
		
	}

	for (int i = 0; i < delayTimesNumber/2; ++i)
	{
		delayTimes.push_back(delayTimes[i] * 4);

	}

	/*for (int i = 0; i < delayTimesNumber/3; ++i)
	{
		delayTimes.push_back((delayTimes[i] + (delayTimes[delayTimesNumber/3 - 1] )* 2));

	}

	for (int i = 0; i < delayTimesNumber / 3; ++i)
	{
		delayTimes.push_back((delayTimes[i] + (delayTimes[delayTimesNumber / 3 - 1]) * 3));

	}*/

	std::sort(delayTimes.begin(), delayTimes.end());
	std::reverse(delayTimes.begin(), delayTimes.end());
	return delayTimes; 
} 
 