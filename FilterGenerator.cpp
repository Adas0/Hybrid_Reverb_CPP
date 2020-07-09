/*
  ==============================================================================

    FiltersGenerator.cpp
    Created: 13 Mar 2020 6:18:59pm
    Author:  Adam

  ==============================================================================
*/

#include "FilterGenerator.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include <math.h>

int FilterGenerator::getFilterCutoffFrequency(int& lowBorder, int& highBorder)
{
	return Random::getSystemRandom().nextInt(Range<int>(lowBorder, highBorder));
}

void FilterGenerator::prepareBandPass(double sampleRate, int samplesPerBlock, int numChannels, dsp::ProcessSpec spec)
{
	for (int filter = 0; filter < filtersNumber; ++filter)
		bandpassCutoffFrequenciesRight.push_back(20000);

	int lowBandFreq = 2000, highBandFreq = 3000;

	for (int filter = ceil(numberDelayLines / 3); filter < ceil(2 * filtersNumber / 3); ++filter)
		bandpassCutoffFrequenciesRight[filter] = getFilterCutoffFrequency(lowBandFreq, highBandFreq);


	for (int filter = 0; filter < filtersNumber; ++filter)
	{
		bandPassFilterRight[filter].prepare(spec);
		bandPassFilterRight[filter].reset();
		*(bandPassFilterRight[filter]).state = *dsp::IIR::Coefficients<float>::makeBandPass(sampleRate, bandpassCutoffFrequenciesRight[filter], 1.0f);
	}
}


void FilterGenerator::prepareNoiseFilters(double sampleRate, int samplesPerBlock, int numChannels, dsp::ProcessSpec spec)
{

	int lowBandFreq = 40, highBandFreq = 900;

	for (int filter = 0; filter < numberDelayLines; ++filter)
		noiseFiltersFrequencies.push_back(getFilterCutoffFrequency(lowBandFreq, highBandFreq));


	for (int filter = 0; filter < filtersNumber; ++filter)
	{
		noiseFilters[filter].prepare(spec);
		noiseFilters[filter].reset();
		*(noiseFilters[filter]).state = *dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, noiseFiltersFrequencies[filter], 1.0f);
	}

	std::sort(noiseFiltersFrequencies.begin(), noiseFiltersFrequencies.end());
	std::reverse(noiseFiltersFrequencies.begin(), noiseFiltersFrequencies.end());

}

void FilterGenerator::prepare(double sampleRate, int samplesPerBlock, int numChannels)
{
	dsp::ProcessSpec spec;
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = numChannels;

	prepareBandPass(sampleRate, samplesPerBlock, numChannels, spec);
	prepareNoiseFilters(sampleRate, samplesPerBlock, numChannels, spec);
	
	allPassFilter.prepare(spec);
	allPassFilter.reset();
	*(allPassFilter).state = *dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 15000.0f);

	filtersNumber = numberDelayLines;
	lowBorderFilterFrequency = 40;
	highBorderFilterFrequency = 2000;
	for (int filter = 0; filter < filtersNumber; ++filter)
	{
		//if (filtersNumber <= numberDelayLines) 
			lowPassCutoffFrequenciesLeft.push_back(getFilterCutoffFrequency(lowBorderFilterFrequency, highBorderFilterFrequency));
		//lowPassCutoffFrequenciesLeft.push_back(2000);
	}

	lowPassCutoffFrequenciesLeft[numberDelayLines - 1] = 20000;

	std::sort(lowPassCutoffFrequenciesLeft.begin(), lowPassCutoffFrequenciesLeft.end());
	//std::reverse(lowPassCutoffFrequenciesLeft.begin(), lowPassCutoffFrequenciesLeft.end());

	lowPassCutoffFrequenciesLeft[lowPassCutoffFrequenciesLeft.size()-1] = 20000;
	/*filterCutoffFrequencies[1] = 20000;
	filterCutoffFrequencies[2] = 20000;*/

	//lowPassCutoffFrequenciesLeft[0] = 10;
	/*lowPassCutoffFrequenciesLeft[1] = 20000;
	lowPassCutoffFrequenciesLeft[2] = 10000;
	lowPassCutoffFrequenciesLeft[3] = 10000;*/

	//lowPassCutoffFrequenciesLeft[0] = 50;
	
	for (int filter = 0; filter < filtersNumber; ++filter)
	{
		lowPassFilterLeft[filter].prepare(spec);
		lowPassFilterLeft[filter].reset();
		*(lowPassFilterLeft[filter]).state = *dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, lowPassCutoffFrequenciesLeft[filter], 1.0f);
	}


	int asd = 50;
	int sdf = 1900;
	for (int filter = 0; filter < filtersNumber; ++filter)
	{
		//if (filtersNumber <= numberDelayLines)
		lowPassCutoffFrequenciesRight.push_back(getFilterCutoffFrequency(asd, sdf));
		//lowPassCutoffFrequenciesRight.push_back(2000);
	}

	lowPassCutoffFrequenciesRight[numberDelayLines - 1] = 20000;
	//lowPassCutoffFrequenciesRight = lowPassCutoffFrequenciesLeft;

	std::sort(lowPassCutoffFrequenciesRight.begin(), lowPassCutoffFrequenciesRight.end());
	//std::reverse(lowPassCutoffFrequenciesRight.begin(), lowPassCutoffFrequenciesRight.end());

	//filterCutoffFrequencies[filterCutoffFrequencies.size()-1] = 20000;
	/*filterCutoffFrequencies[1] = 20000;
	filterCutoffFrequencies[2] = 20000;*/

	//lowPassCutoffFrequenciesRight[0] = 10;
	/*lowPassCutoffFrequenciesRight[1] = 20000;
	lowPassCutoffFrequenciesRight[2] = 10000;
	lowPassCutoffFrequenciesRight[3] = 10000;*/


	

	for (int filter = 0; filter < filtersNumber; ++filter)
	{
		lowPassFilterRight[filter].prepare(spec);
		lowPassFilterRight[filter].reset();
		*(lowPassFilterRight[filter]).state = *dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, lowPassCutoffFrequenciesRight[filter], 1.0f);
	}

}
//hmm, przy takim układzie wszystkie obiekty klasy FilterGenerator będą miały taką samą tą tablicę lowPassFilter? Bo chyba
//jak przypisuję w funkcji prepare wartości do tej tablicy, to ta tablica jest przypisana tak jakby do klasy?