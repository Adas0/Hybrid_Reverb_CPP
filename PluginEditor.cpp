/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Circular_attemptAudioProcessorEditor::Circular_attemptAudioProcessorEditor (Circular_attemptAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (200, 100);

	wetDrySlider.setSliderStyle(Slider::Rotary);
	wetDrySlider.setRange(0.0, 1, 0.01);
	wetDrySlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 1);
	wetDrySlider.setPopupDisplayEnabled(true, false, this);
	wetDrySlider.setTextValueSuffix(" ");
	wetDrySlider.setValue(0.0);
	addAndMakeVisible(&wetDrySlider);
	wetDrySlider.addListener(this);

	ILDwetSlider.setSliderStyle(Slider::Rotary);
	ILDwetSlider.setRange(0.0, 1.0, 0.01);
	ILDwetSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 1);
	ILDwetSlider.setPopupDisplayEnabled(true, false, this);
	ILDwetSlider.setTextValueSuffix(" ");
	ILDwetSlider.setValue(1.0);
	addAndMakeVisible(&ILDwetSlider);
	ILDwetSlider.addListener(this);

	//wetDrySlider

}

Circular_attemptAudioProcessorEditor::~Circular_attemptAudioProcessorEditor()
{
}

//==============================================================================
void Circular_attemptAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    //g.setColour (Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);

	//reverbLengthSlider.setBounds(200, 0, 200, 200);
}

void Circular_attemptAudioProcessorEditor::resized()
{
	//processor.reverbLength = reverbLengthSlider.getValue();
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	wetDrySlider.setBounds(0, 0, 100, 100);
	ILDwetSlider.setBounds(100, 0, 100, 100);
}

void Circular_attemptAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
	//processor.reverbLength = reverbLengthSlider.getValue();
	////processor.reverbSize = reverbSizeSlider.getValue();
	//processor.firstRefTime = reverbSizeSlider.getValue();
	processor.reverbEngine.wetDry = wetDrySlider.getValue();
	processor.reverbEngine.reflectionAmplitude = 1 - wetDrySlider.getValue();

	processor.reverbEngine.ILDwet = ILDwetSlider.getValue();
}
