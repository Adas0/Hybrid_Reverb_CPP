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

//w tej klasie mogą być wykonywane wszystkie operacje związane z tworzeniem filtrów. Tylko czy wtedy będzie łatwo przesłać te dane
//do ewentualnej modyfikacji użytkownikowi w Editor
