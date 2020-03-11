/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class Circular_attemptAudioProcessorEditor  : public AudioProcessorEditor, private Slider::Listener
{
public:
    Circular_attemptAudioProcessorEditor (Circular_attemptAudioProcessor&);
    ~Circular_attemptAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	void sliderValueChanged(Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Circular_attemptAudioProcessor& processor;
	Slider reverbLengthSlider, reverbSizeSlider;
	//Slider firstRelectionTimeSlider;

	Slider filterCutOffDial;
	Slider filteResDial;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> filterCutoffValue;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> filterResValue;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Circular_attemptAudioProcessorEditor)
};
