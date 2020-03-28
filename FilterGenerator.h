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
	dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> lowPassFilter[numberDelayLines];
//private:
	std::vector<int>filterCutoffFrequencies;
	int filtersNumber;
	int lowBorderFilterFrequency, highBorderFilterFrequency;
	
};

//w tej klasie mogą być wykonywane wszystkie operacje związane z tworzeniem filtrów. Tylko czy wtedy będzie łatwo przesłać te dane
//do ewentualnej modyfikacji użytkownikowi w Editor
