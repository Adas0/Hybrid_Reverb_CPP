/*
  ==============================================================================

    SpatialMaker.cpp
    Created: 27 Mar 2020 12:45:10pm
    Author:  Adam

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
	level = (level - 0.5) / 5 - 0.2;
	return level;
}

//prawdopodobnie ta funkcja powinna zwracać wektor, i w ogóle być wywoływana w klasie ReverbEngine. Bo wtedy mam możliwość
//stworzenia kilku obiektów klasy SpatialMaker 
void SpatialMaker::createITDarray()
{
	ITDCoefficients.clear();
	for (int line = 0; line < numberDelayLines; ++line)
	{
		//if (line > 1)
			ITDCoefficients.push_back(getITD(-40, 40));
		//else
		//	ITDCoefficients.push_back(/*getITD(-2, 2)*/0);
			
	}
	ITDCoefficients[numberDelayLines - 1] = 0;
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
	/*for (int line = 0; line < numberDelayLines; ++line)
	{
		ILDCoefficients.push_back(ITDCoefficients[line] / 15);
	}*/
}

std::vector<std::pair<float, int>> SpatialMaker::getSpatialInfo()
{
	std::vector<std::pair<float, int>> spatialInfo;


	return spatialInfo;
}

void SpatialMaker::prepare()
{
	//createITDarray();
	createILDarray();
	createILD_test();

}


