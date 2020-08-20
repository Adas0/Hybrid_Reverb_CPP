/*
  ==============================================================================

    FiltersGenerator.h
    Created: 13 Mar 2020 6:18:42pm
    Author:  Adam

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "ReverbElement.h"

class FilterGenerator: public ReverbElement
{
public:
	void prepare(double sampleRate, int samplesPerBlock, int numChannels);
	int getFilterCutoffFrequency(int& lowBorder, int& highBorder);
	dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> allPassFilter;

	dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> lowPassFilterLeft[numberDelayLines];
	dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> lowPassFilterRight[numberDelayLines];

	dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> bandPassFilterLeft[numberDelayLines];
	dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> bandPassFilterRight[numberDelayLines];

	dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> noiseFilters[numberDelayLines];

	dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> constantFiltersLeft[numberDelayLines];
	dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> constantFiltersRight[numberDelayLines];

//private:
	std::vector<int>lowPassCutoffFrequenciesLeft;
	std::vector<int>lowPassCutoffFrequenciesRight;
	int filtersNumber;
	int lowBorderFilterFrequency, highBorderFilterFrequency;

	std::vector<int>bandpassCutoffFrequenciesRight;
	void prepareBandPass(double sampleRate, int samplesPerBlock, int numChannels, dsp::ProcessSpec spec);

	std::vector<int>noiseFiltersFrequencies;
	void prepareNoiseFilters(double sampleRate, int samplesPerBlock, int numChannels, dsp::ProcessSpec spec);

};

//w tej klasie mogą być wykonywane wszystkie operacje związane z tworzeniem filtrów. Tylko czy wtedy będzie łatwo przesłać te dane
//do ewentualnej modyfikacji użytkownikowi w Editor - Tak.
