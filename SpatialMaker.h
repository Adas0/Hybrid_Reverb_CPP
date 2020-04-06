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
	int getITD();
	float getILD();
	void prepare();
	std::vector<int>ITDCoefficients;
	std::vector<int>ILDCoefficients;
	void createITDarray();
	void createILDarray();

	void createILD_test();

	std::vector < std::pair<float, int>> getSpatialInfo();

};