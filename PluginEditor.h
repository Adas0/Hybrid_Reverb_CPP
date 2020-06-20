/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "ReverbElement.h"

//==============================================================================
/**
*/
class Circular_attemptAudioProcessorEditor  : public AudioProcessorEditor, private Slider::Listener, public ReverbElement
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
	Slider /*wetDrySlider,*/ ILDwetSlider, trueWetDry;
	//Slider firstRelectionTimeSlider;

	Slider filterCutOffDial;
	Slider filteResDial;
	Slider reverbSizeSlider;
	//Slider wetDrySlider;
	/*ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> filterCutoffValue;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> filterResValue;*/
	//Slider ;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Circular_attemptAudioProcessorEditor)
};
