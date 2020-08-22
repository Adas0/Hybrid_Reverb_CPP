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


void FilterGenerator::prepareNoiseFilters(double sampleRate, 
			int samplesPerBlock, int numChannels, dsp::ProcessSpec spec)
{

	int lowBandFreq = 40, highBandFreq = 1050;

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

	//prepareBandPass(sampleRate, samplesPerBlock, numChannels, spec);
	prepareNoiseFilters(sampleRate, samplesPerBlock, numChannels, spec);
	
	/*allPassFilter.prepare(spec);
	allPassFilter.reset();
	*(allPassFilter).state = *dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 15000.0f);
*/

	filtersNumber = numberDelayLines;
	lowBorderFilterFrequency = 50;
	highBorderFilterFrequency = 2800;
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


	int rightLowHighpassFreq = 40;
	int rightHighHighpassFreq = 3000;
	for (int filter = 0; filter < filtersNumber; ++filter)
	{
		lowPassCutoffFrequenciesRight.push_back(getFilterCutoffFrequency(
								rightLowHighpassFreq, rightHighHighpassFreq));

	}

	lowPassCutoffFrequenciesRight[0] = 20000.0f;
	sort(lowPassCutoffFrequenciesRight.begin(), lowPassCutoffFrequenciesRight.end());

	for (int filter = 0; filter < filtersNumber; ++filter)
	{
		lowPassFilterRight[filter].prepare(spec);
		lowPassFilterRight[filter].reset();
		*(lowPassFilterRight[filter]).state = *dsp::IIR::Coefficients<float>::
				makeLowPass(sampleRate, lowPassCutoffFrequenciesRight[filter], 1.0f);
	}

	///////////////////////////////////
	int asd = 500;
	int qwe = 500;
	std::vector<int>qqq;
	for (int filter = 0; filter < filtersNumber; ++filter)
	{
		qqq.push_back(getFilterCutoffFrequency(
			asd, qwe));

	}

	for (int filter = 0; filter < filtersNumber; ++filter)
	{
		constantFiltersLeft[filter].prepare(spec);
		constantFiltersLeft[filter].reset();
		*(constantFiltersLeft[filter]).state = *dsp::IIR::Coefficients<float>::
			makeLowPass(sampleRate, qqq[filter], 1.0f);

		constantFiltersRight[filter].prepare(spec);
		constantFiltersRight[filter].reset();
		*(constantFiltersRight[filter]).state = *dsp::IIR::Coefficients<float>::
			makeLowPass(sampleRate, qqq[filter], 1.0f);
	}

}
//hmm, przy takim układzie wszystkie obiekty klasy FilterGenerator będą miały taką samą tą tablicę lowPassFilter? Bo chyba
//jak przypisuję w funkcji prepare wartości do tej tablicy, to ta tablica jest przypisana tak jakby do klasy?