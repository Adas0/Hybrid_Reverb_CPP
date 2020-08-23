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
class Circular_attemptAudioProcessorEditor  : public AudioProcessorEditor, private Slider::Listener, 
						/*private ComboBox::Listener,*/ private Label::Listener, public ReverbElement
{
public:
    Circular_attemptAudioProcessorEditor (Circular_attemptAudioProcessor&);
    ~Circular_attemptAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	void sliderValueChanged(Slider* slider) override;

	void Label::Listener::labelTextChanged(Label * labelThatHasChanged);
	//void onChange(ComboBox* box) override;
	
	void filtersChange();
	void noiseToggleStateChange();
	void onStateSwitchNoise();
	void onStateSwitchITD();
	void onStateSwitchILD();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Circular_attemptAudioProcessor& processor;
	Slider /*wetDrySlider,*/ ILDwetSlider, trueWetDry;
	//Slider firstRelectionTimeSlider;

	//Slider filterCutOffDial;
	Slider filteResDial;
	Slider reverbSizeSlider;
	Slider noiseIntensitySlider;
	Slider firstRefSlider;
	Slider lateralAmplitudeDifferenceSlider;

	ComboBox filtersChoose;

	ToggleButton noiseToggle, ITD_toggle, ILD_toggle;

	Label wetDryLabel, reverbLengthLabel, lateralRefsLabel, firstRefTimeLabel;
	Label wetDryLabelValues, reverbLengthLabelValues, lateralRefsLabelValues, firstRefTimeLabelValues;

	Label ITD_label, ILD_label, noiseLabel;
	//Slider directSoundCutoffSlider;
	//Slider ITDslider;
	//Slider wetDrySlider;
	/*ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> filterCutoffValue;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> filterResValue;*/
	//Slider ;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Circular_attemptAudioProcessorEditor)
};
