/*
  ==============================================================================

    DelayTimesGenerator.h
    Created: 16 Feb 2020 11:52:05am
    Author:  Adam

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>

class DelayTimesGenerator
{
public:
	DelayTimesGenerator() {}

	int highDelayTime, lowDelayTime;
	std::vector<int> getDelayTimes(int & delayTimesNumber, int& lowTime, int& highTime);
	int generateRandomNumber(int& lowTime, int& highTime);

};