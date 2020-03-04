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

	int generateRandomNumber();
	std::vector<int> getDelayTimes(int & delayTimesNumber);
	std::vector<float> getNoiseCoefficients(int & numCoefficients);
};