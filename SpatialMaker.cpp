/*
  ==============================================================================

    SpatialMaker.cpp
    Created: 27 Mar 2020 12:45:10pm
    Author:  Adam Korytowski

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "SpatialMaker.h"


int SpatialMaker::getITD(int minus, int plus)
{
	return Random::getSystemRandom().nextInt(Range<int>(minus, plus));
}

float SpatialMaker::getILD()
{
	float level = Random::getSystemRandom().nextFloat();
	level = (level - 0.5) / 5;
	return level;
}

void SpatialMaker::createITDarray()
{
	ITDCoefficients.clear();
	for (int line = 0; line < numberDelayLines; ++line)
	{
		ITDCoefficients.push_back(Random::getSystemRandom().
								nextInt(Range<int>(-40, 40)));
			
	}
}

void SpatialMaker::createILDarray()
{
	for (int line = 0; line < numberDelayLines; ++line)
	{
		ILDCoefficients.push_back(getILD());
	}
} 

void SpatialMaker::createILD_test()
{
	createITDarray();
}

std::vector<std::pair<float, int>> SpatialMaker::getSpatialInfo()
{
	std::vector<std::pair<float, int>> spatialInfo;


	return spatialInfo;
}

void SpatialMaker::prepare()
{
	createILDarray();
	createILD_test();

}
