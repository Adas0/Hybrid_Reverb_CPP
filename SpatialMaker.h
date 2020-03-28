/*
  ==============================================================================

    SpatialMaker.h
    Created: 27 Mar 2020 12:45:00pm
    Author:  Adam

  ==============================================================================
*/

#pragma once
#include <vector>
#include "ReverbElement.h"

class SpatialMaker: public ReverbElement
{
public:
	int getITDTime();
	void prepare();
	std::vector<int>ITDCoefficients;
	void createITDarray();
};