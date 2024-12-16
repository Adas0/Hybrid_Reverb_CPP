/*
  ==============================================================================

//Date: 2020
//Author: Juce + Adam Korytowski

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Circular_attemptAudioProcessorEditor::Circular_attemptAudioProcessorEditor(Circular_attemptAudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p)
{
    	setSize (600, 400);
	
	trueWetDry.setSliderStyle(Slider::Rotary);
	trueWetDry.setRange(0.0f, 1.0f, 0.01f);
	trueWetDry.setTextBoxStyle(Slider::TextBoxBelow, true, getWidth(), 30);
	trueWetDry.setPopupDisplayEnabled(true, false, this);
	//trueWetDry.setTextValueSuffix(" ");
	trueWetDry.setValue(1.0f);
	addAndMakeVisible(&trueWetDry);
	trueWetDry.addListener(this);

	reverbSizeSlider.setSliderStyle(Slider::Rotary);
	reverbSizeSlider.setRange(0, 1800, 1);
	//reverbSizeSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 1);
	reverbSizeSlider.setPopupDisplayEnabled(true, false, this);
	reverbSizeSlider.setTextValueSuffix(" ms");
	reverbSizeSlider.setValue(1800);
	reverbSizeSlider.setTextBoxStyle(Slider::TextBoxBelow, true, getWidth(), 30);
	addAndMakeVisible(&reverbSizeSlider);
	reverbSizeSlider.addListener(this);

	firstRefSlider.setSliderStyle(Slider::Rotary);
	firstRefSlider.setRange(0, 300, 1);
	firstRefSlider.setTextBoxStyle(Slider::TextBoxBelow, true, getWidth(), 30);
	firstRefSlider.setPopupDisplayEnabled(false, false, this);
	firstRefSlider.setTextValueSuffix(" ms");
	firstRefSlider.setValue(100);
	addAndMakeVisible(&firstRefSlider);
	firstRefSlider.addListener(this);

	lateralAmplitudeDifferenceSlider.setSliderStyle(Slider::Rotary);
	lateralAmplitudeDifferenceSlider.setRange(0.0, 1.0, 0.01);
	lateralAmplitudeDifferenceSlider.setTextBoxStyle(Slider::TextBoxBelow, true, getWidth(), 30);
	lateralAmplitudeDifferenceSlider.setPopupDisplayEnabled(true, false, this);
	lateralAmplitudeDifferenceSlider.setValue(0.0);
	addAndMakeVisible(&lateralAmplitudeDifferenceSlider);
	lateralAmplitudeDifferenceSlider.addListener(this);
	

	addAndMakeVisible(&filtersChoose);
	filtersChoose.addItem("Constant filters", 1);
	filtersChoose.addItem("Delay dependent filters", 2);

	filtersChoose.onChange = [this] { filtersChange(); };
	filtersChoose.setSelectedId(2);

	ITD_toggle.setToggleState(true, true);
	ITD_toggle.onClick = [this] {Circular_attemptAudioProcessorEditor::onStateSwitchITD(); };
	addAndMakeVisible(&ITD_toggle);
	
	ILD_toggle.setToggleState(false, false);
	ILD_toggle.onClick = [this] {Circular_attemptAudioProcessorEditor::onStateSwitchILD(); };
	addAndMakeVisible(&ILD_toggle);

	noiseToggle.setToggleState(false, false);
	noiseToggle.onClick = [this] {Circular_attemptAudioProcessorEditor::onStateSwitchNoise(); };
	addAndMakeVisible(&noiseToggle);

	wetDryLabel.setText("Wet/Dry", dontSendNotification);
	wetDryLabel.addListener(this);
	addAndMakeVisible(wetDryLabel);

	addAndMakeVisible(reverbLengthLabel);
	reverbLengthLabel.setText("Length", dontSendNotification);
	reverbLengthLabel.addListener(this);

	addAndMakeVisible(firstRefTimeLabel);
	firstRefTimeLabel.setText("First reflection time", dontSendNotification);
	firstRefTimeLabel.addListener(this);
	firstRefTimeLabel.attachToComponent(&firstRefSlider, false);

	addAndMakeVisible(lateralRefsLabel);
	lateralRefsLabel.setText("First ref. amp. diff.", dontSendNotification);
	lateralRefsLabel.addListener(this);
	lateralRefsLabel.attachToComponent(&lateralAmplitudeDifferenceSlider, false);

	addAndMakeVisible(ITD_label);
	ITD_label.setText("ITD", dontSendNotification);
	ITD_label.addListener(this);

	addAndMakeVisible(ILD_label);
	ILD_label.setText("ILD", dontSendNotification);
	ILD_label.addListener(this);

	addAndMakeVisible(noiseLabel);
	noiseLabel.setText("Noise", dontSendNotification);
	noiseLabel.addListener(this);
}

void Circular_attemptAudioProcessorEditor::labelTextChanged(Label * labelThatHasChanged)
{
	auto SliderValue = firstRefSlider.getValue();

}


void Circular_attemptAudioProcessorEditor::onStateSwitchILD()
{
	auto state = ILD_toggle.getToggleState();
	processor.reverbEngine.ILD_on = (bool)state;
}

void Circular_attemptAudioProcessorEditor::onStateSwitchITD()
{
	auto state = ITD_toggle.getToggleState();
	processor.reverbEngine.ITD_on = (bool)state;
}

void Circular_attemptAudioProcessorEditor::onStateSwitchNoise()
{
	auto state = noiseToggle.getToggleState();
	processor.reverbEngine.noiseOn = (bool)state;
}

void Circular_attemptAudioProcessorEditor::noiseToggleStateChange()
{

}

void Circular_attemptAudioProcessorEditor::filtersChange()
{
	switch (filtersChoose.getSelectedId())
	{
		case 1: processor.reverbEngine.filtersOption = 0; break;
		case 2: processor.reverbEngine.filtersOption = 1; break;
		default: break;
	}
}

Circular_attemptAudioProcessorEditor::~Circular_attemptAudioProcessorEditor()
{
}

//==============================================================================
void Circular_attemptAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)

	g.fillAll(Colour::Colour(77, 0, 77));
}

void Circular_attemptAudioProcessorEditor::resized()
{
	trueWetDry.setBounds(50, 50, 120, 120);
	reverbSizeSlider.setBounds(250, 50, 120, 120);
	firstRefSlider.setBounds(50, 250, 120, 120);
	lateralAmplitudeDifferenceSlider.setBounds(250, 250, 120, 120);

	filtersChoose.setBounds(400, 150, 180, 30);

	//TOGGLES//ITD_toggle
	ITD_toggle.setBounds(410, 200, 30, 30);
	ILD_toggle.setBounds(480, 200, 30, 30);
	noiseToggle.setBounds(550, 200, 30, 30);
	//////////

	//LABELS//
	wetDryLabel.setBounds(77, 30, getWidth(), 30);
	reverbLengthLabel.setBounds(281, 30, getWidth(), 30);
	firstRefTimeLabel.setBounds(35, 230, getWidth(), 30);
	lateralRefsLabel.setBounds(235, 230, getWidth(), 30);

	ITD_label.setBounds(405, 230, 50, 30);
	ILD_label.setBounds(475, 230, 50, 30);
	noiseLabel.setBounds(540, 230, 50, 30);
	//////////
}

void Circular_attemptAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
	processor.reverbEngine.mWetDry = trueWetDry.getValue();
	processor.reverbEngine.reverbSize = reverbSizeSlider.getValue();
	processor.reverbEngine.noiseIntensity = noiseIntensitySlider.getValue();
	processor.reverbEngine.firstRefTime = firstRefSlider.getValue();
	processor.reverbEngine.lateralAmplitudeDifference = lateralAmplitudeDifferenceSlider.getValue();

}
