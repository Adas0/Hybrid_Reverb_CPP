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
	int getFilterCutoffFrequency(int& lowBorder, int& highBorder);
};
