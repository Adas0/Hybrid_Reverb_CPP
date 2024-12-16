/*
  ==============================================================================

    FiltersGenerator.cpp
    Created: 13 Mar 2020 6:18:59pm
    Author:  Adam Korytowski

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


void FilterGenerator::prepareNoiseFilters(double sampleRate, 
			int samplesPerBlock, int numChannels, dsp::ProcessSpec spec)
{

	int lowBandFreq = 40, highBandFreq = 750;

	for (int filter = 0; filter < numberDelayLines; ++filter)
		noiseFiltersFrequencies.push_back(getFilterCutoffFrequency
												(lowBandFreq, highBandFreq));


	for (int filter = 0; filter < filtersNumber; ++filter)
	{
		noiseFilters[filter].prepare(spec);
		noiseFilters[filter].reset();
		*(noiseFilters[filter]).state = *dsp::IIR::Coefficients<float>::
					makeLowPass(sampleRate, noiseFiltersFrequencies[filter], 1.0f);
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

	prepareNoiseFilters(sampleRate, samplesPerBlock, numChannels, spec);
	
	filtersNumber = numberDelayLines;
	lowBorderFilterFrequency = 100;
	highBorderFilterFrequency = 3000;
	for (int filter = 0; filter < filtersNumber; ++filter)
	{
		lowPassCutoffFrequenciesLeft.push_back(getFilterCutoffFrequency(
							lowBorderFilterFrequency, highBorderFilterFrequency));
	}

	lowPassCutoffFrequenciesLeft[0] = 20000.0f;
	sort(lowPassCutoffFrequenciesLeft.begin(), lowPassCutoffFrequenciesLeft.end()); 

	for (int filter = 0; filter < filtersNumber; ++filter)
	{
		lowPassFilterLeft[filter].prepare(spec);
		lowPassFilterLeft[filter].reset();
		*(lowPassFilterLeft[filter]).state = *dsp::IIR::Coefficients<float>::
					makeLowPass(sampleRate, lowPassCutoffFrequenciesLeft[filter], 1.0f);
	}

	int rightLowHighpassFreq = 80;
	int rightHighHighpassFreq = 3800;
	for (int filter = 0; filter < filtersNumber; ++filter)
	{
		lowPassCutoffFrequenciesRight.push_back(getFilterCutoffFrequency(
								rightLowHighpassFreq, rightHighHighpassFreq));
	}

	lowPassCutoffFrequenciesRight[0] = 20000.0f;
	sort(lowPassCutoffFrequenciesRight.begin(), lowPassCutoffFrequenciesRight.end());
	lowPassCutoffFrequenciesRight[lowPassCutoffFrequenciesRight.size() - 2] = 15000.0f;

	for (int filter = 0; filter < filtersNumber; ++filter)
	{
		lowPassFilterRight[filter].prepare(spec);
		lowPassFilterRight[filter].reset();
		*(lowPassFilterRight[filter]).state = *dsp::IIR::Coefficients<float>::
				makeLowPass(sampleRate, lowPassCutoffFrequenciesRight[filter], 1.0f);
	}

	///////////////////////////////////
	int LowFrequency = 500;
	int HighFrequency = 500;
	std::vector<int>CutoffFrequencies;
	for (int filter = 0; filter < filtersNumber; ++filter)
	{
		CutoffFrequencies.push_back(getFilterCutoffFrequency(
			LowFrequency, HighFrequency));

	}

	for (int filter = 0; filter < filtersNumber; ++filter)
	{
		constantFiltersLeft[filter].prepare(spec);
		constantFiltersLeft[filter].reset();
		*(constantFiltersLeft[filter]).state = *dsp::IIR::Coefficients<float>::
			makeLowPass(sampleRate, CutoffFrequencies[filter], 1.0f);

		constantFiltersRight[filter].prepare(spec);
		constantFiltersRight[filter].reset();
		*(constantFiltersRight[filter]).state = *dsp::IIR::Coefficients<float>::
			makeLowPass(sampleRate, CutoffFrequencies[filter], 1.0f);
	}

}
