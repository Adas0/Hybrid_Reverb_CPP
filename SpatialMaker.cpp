/*
  ==============================================================================

    SpatialMaker.cpp
    Created: 27 Mar 2020 12:45:10pm
    Author:  Adam

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "SpatialMaker.h"


int SpatialMaker::getITDTime()
{
	return Random::getSystemRandom().nextInt(Range<int>(-9, 9));
}

//prawdopodobnie ta funkcja powinna zwracać wektor, i w ogóle być wywoływana w klasie ReverbEngine. Bo wtedy mam możliwość
//stworzenia kilku obiektów klasy SpatialMaker 
void SpatialMaker::createITDarray()
{
	for (int line = 0; line < numberDelayLines; ++line)
	{
		ITDCoefficients.push_back(getITDTime());
	}
}

void SpatialMaker::prepare()
{
	createITDarray();
}
