/*
  ==============================================================================

    SpatialMaker.cpp
    Created: 27 Mar 2020 12:45:10pm
    Author:  Adam

  ==============================================================================
*/

#include "SpatialMaker.h"


int SpatialMaker::getITDTime()
{
	return Random::getSystemRandom().nextInt(Range<int>(-15, 15));
}

void SpatialMaker::addITD()
{
	for (int filter = 0; filter < filtersNumber; ++filter)
	{
		ITDCoefficients.push_back(getITDTime());
	}
}