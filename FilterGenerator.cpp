/*
  ==============================================================================

    FiltersGenerator.cpp
    Created: 13 Mar 2020 6:18:59pm
    Author:  Adam

  ==============================================================================
*/

#include "FilterGenerator.h"
#include "../JuceLibraryCode/JuceHeader.h"


int FilterGenerator::getFilterCutoffFrequency(int& lowBorder, int& highBorder)
{
	return Random::getSystemRandom().nextInt(Range<int>(lowBorder, highBorder));
}

void FilterGenerator::prepare(double sampleRate, int samplesPerBlock, int numChannels)
{
	dsp::ProcessSpec spec;
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = numChannels;
	
	allPassFilter.prepare(spec);
	allPassFilter.reset();
	*(allPassFilter).state = *dsp::IIR::Coefficients<float>::makeAllPass(sampleRate, 15000.0f);

	filtersNumber = numberDelayLines;
	lowBorderFilterFrequency = 100;
	highBorderFilterFrequency = 1500;
	for (int filter = 0; filter < filtersNumber; ++filter)
	{
		//if (filtersNumber <= numberDelayLines)
			lowPassCutoffFrequenciesLeft.push_back(getFilterCutoffFrequency(lowBorderFilterFrequency, highBorderFilterFrequency));
	}

	//filterCutoffFrequencies[filterCutoffFrequencies.size()-1] = 20000;
	/*filterCutoffFrequencies[1] = 20000;
	filterCutoffFrequencies[2] = 20000;*/

	for (int filter = 0; filter < filtersNumber; ++filter)
	{
		lowPassFilterLeft[filter].prepare(spec);
		lowPassFilterLeft[filter].reset();
		*(lowPassFilterLeft[filter]).state = *dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, lowPassCutoffFrequenciesLeft[filter], 1.0f);
	}


	int asd = 2000;
	int sdf = 5000;
	for (int filter = 0; filter < filtersNumber; ++filter)
	{
		//if (filtersNumber <= numberDelayLines)
		lowPassCutoffFrequenciesRight.push_back(getFilterCutoffFrequency(asd, sdf));
	}

	//filterCutoffFrequencies[filterCutoffFrequencies.size()-1] = 20000;
	/*filterCutoffFrequencies[1] = 20000;
	filterCutoffFrequencies[2] = 20000;*/

	for (int filter = 0; filter < filtersNumber; ++filter)
	{
		lowPassFilterRight[filter].prepare(spec);
		lowPassFilterRight[filter].reset();
		*(lowPassFilterRight[filter]).state = *dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, lowPassCutoffFrequenciesRight[filter], 1.0f);
	}



}
//hmm, przy takim układzie wszystkie obiekty klasy FilterGenerator będą miały taką samą tą tablicę lowPassFilter? Bo chyba
//jak przypisuję w funkcji prepare wartości do tej tablicy, to ta tablica jest przypisana tak jakby do klasy?