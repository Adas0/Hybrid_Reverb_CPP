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


std::vector<int> DelayTimesGenerator::getDelayTimes(int & delayTimesNumber, int& lowTime, int& highTime, int& firstRefTime)
{
	std::vector<int>delayTimes;
	delayTimes.clear();
	for (int i = 0; i < delayTimesNumber; ++i)
	{ 
		//delayTimes.push_back(firstRefTime + generateRandomNumber(lowTime, highTime));
		delayTimes.push_back(this->delayTimesPrime_[i * 4] /*+ firstRefTime*/);
		
	}

	//for (int i = 0; i < delayTimesNumber / 2; ++i)
	//{ 
	//	//delayTimes.push_back(firstRefTime + generateRandomNumber(lowTime, highTime));
	//	delayTimes.push_back(this->delayTimesPrime_[i * 2] + firstRefTime);
	//	
	//}

	//for (int i = 3 * delayTimesNumber / 4; i < delayTimesNumber; ++i)
	//{
	//	//delayTimes.push_back(firstRefTime + generateRandomNumber(lowTime, highTime));
	//	delayTimes.push_back(this->delayTimesPrime_[i * 2] + firstRefTime);
	//	delayTimes.push_back(this->delayTimesPrime_[i * 2 + 1] + firstRefTime);

	//}

	//auto max = *max_element(std::begin(delayTimes), std::end(delayTimes));
	//int a = 0;
	//int b = 40;
	//for (int i = 0; i < delayTimesNumber / 2; ++i)
	//{
	//	//delayTimes.push_back(delayTimes[i] + max);
	//	delayTimes.push_back(firstRefTime + generateRandomNumber(a, b));

	//}

	/*
	for (int i = 0; i < delayTimesNumber/4; ++i)
	{
		delayTimes.push_back(delayTimes[i] + max);

	}

	for (int i = 0; i < delayTimesNumber / 4; ++i)
	{
		delayTimes.push_back(delayTimes[i] + 2 *max);

	}
	for (int i = 0; i < delayTimesNumber / 4; ++i)
	{
		delayTimes.push_back(delayTimes[i] + 3 *max);

	}*/
	/*for (int i = 0; i < delayTimesNumber; ++i) 
	{
		delayTimes.push_back(this->delayTimesPrime_[4 * i]);

	}*/

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
 