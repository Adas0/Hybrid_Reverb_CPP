/*
  ==============================================================================

    SpatialMaker.h
    Created: 27 Mar 2020 12:45:00pm
    Author:  Adam Korytowski

  ==============================================================================
*/

#pragma once
#include <vector>
#include "ReverbElement.h"

class SpatialMaker: public ReverbElement
{
public:
	int getITD(int minus, int plus);
	float getILD();
	void prepare();
	std::vector<int>ITDCoefficients;
	std::vector<float>ILDCoefficients;
	void createITDarray();
	void createILDarray();

	void createILD_test();
	int ITDasd;
	std::vector < std::pair<float, int>> getSpatialInfo();

};
