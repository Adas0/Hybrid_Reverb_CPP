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
	highBorderFilterFrequency = 6000;
	for (int filter = 0; filter < filtersNumber; ++filter)
	{
		//if (filtersNumber <= numberDelayLines)
			filterCutoffFrequencies.push_back(getFilterCutoffFrequency(lowBorderFilterFrequency, highBorderFilterFrequency));
	}

	filterCutoffFrequencies[0] = 20000;
	filterCutoffFrequencies[1] = 20000;
	filterCutoffFrequencies[2] = 20000;

	for (int filter = 0; filter < filtersNumber; ++filter)
	{
		lowPassFilter[filter].prepare(spec);
		lowPassFilter[filter].reset();
		*(lowPassFilter[filter]).state = *dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, filterCutoffFrequencies[filter], 1.0f);
	}
}
//hmm, przy takim układzie wszystkie obiekty klasy FilterGenerator będą miały taką samą tą tablicę lowPassFilter? Bo chyba
//jak przypisuję w funkcji prepare wartości do tej tablicy, to ta tablica jest przypisana tak jakby do klasy?