/*
  ==============================================================================

    SpatialMaker.cpp
    Created: 27 Mar 2020 12:45:10pm
    Author:  Adam

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "SpatialMaker.h"


int SpatialMaker::getITD()
{
	return Random::getSystemRandom().nextInt(Range<int>(-15, 15));
}

float SpatialMaker::getILD()
{
	float level = Random::getSystemRandom().nextFloat();
	return (level - 0.5) / 3;
}

//prawdopodobnie ta funkcja powinna zwracać wektor, i w ogóle być wywoływana w klasie ReverbEngine. Bo wtedy mam możliwość
//stworzenia kilku obiektów klasy SpatialMaker 
void SpatialMaker::createITDarray()
{
	for (int line = 0; line < numberDelayLines; ++line)
	{
		ITDCoefficients.push_back(getITD());
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
	for (int line = 0; line < numberDelayLines; ++line)
	{
		ILDCoefficients.push_back(ITDCoefficients[line] / 15);
	}
}

std::vector<std::pair<float, int>> SpatialMaker::getSpatialInfo()
{
	std::vector<std::pair<float, int>> spatialInfo;


	return spatialInfo;
}

void SpatialMaker::prepare()
{
	//createITDarray();
	//createILDarray();
	createILD_test();

}


