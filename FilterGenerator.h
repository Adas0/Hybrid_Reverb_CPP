/*
  ==============================================================================

    FiltersGenerator.h
    Created: 13 Mar 2020 6:18:42pm
    Author:  Adam

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class FilterGenerator
{
public:
	int lowBorderFilterFrequency, highBorderFilterFrequency;
	int getFilterCutoffFrequency(int& lowBorder, int& highBorder);
	static const int numberDelayLines = 150;
	dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> allPassFilter;
	dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> lowPassFilter[numberDelayLines];
	std::vector<int>filterCutoffFrequencies;
	int filtersNumber;
	void prepare(double sampleRate, int samplesPerBlock);
};

//w tej klasie mogą być wykonywane wszystkie operacje związane z tworzeniem filtrów. Tylko czy wtedy będzie łatwo przesłać te dane
//do ewentualnej modyfikacji użytkownikowi w Editor
