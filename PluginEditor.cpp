/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Circular_attemptAudioProcessorEditor::Circular_attemptAudioProcessorEditor(Circular_attemptAudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 400);

	//wetDrySlider.setSliderStyle(Slider::Rotary);
	//wetDrySlider.setRange(0.0, 1, 0.01);
	//wetDrySlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 1);
	//wetDrySlider.setPopupDisplayEnabled(true, false, this);
	//wetDrySlider.setTextValueSuffix(" ");
	//wetDrySlider.setValue(0.0);
	//addAndMakeVisible(&wetDrySlider);
	//wetDrySlider.addListener(this);

	/*ILDwetSlider.setSliderStyle(Slider::Rotary);
	ILDwetSlider.setRange(0.0, 1.0, 0.01);
	ILDwetSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 1);
	ILDwetSlider.setPopupDisplayEnabled(true, false, this);
	ILDwetSlider.setTextValueSuffix(" ");
	ILDwetSlider.setValue(1.0);
	addAndMakeVisible(&ILDwetSlider);
	ILDwetSlider.addListener(this);*/

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

	
	//noiseIntensitySlider.setSliderStyle(Slider::Rotary);
	//noiseIntensitySlider.setRange(0.0f, 2.0f, 0.01f);
	//noiseIntensitySlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 1);
	//noiseIntensitySlider.setPopupDisplayEnabled(true, false, this);
	//noiseIntensitySlider.setTextValueSuffix(" ");
	//noiseIntensitySlider.setValue(2.0f);
	////addAndMakeVisible(&noiseIntensitySlider);
	//noiseIntensitySlider.addListener(this);

	firstRefSlider.setSliderStyle(Slider::Rotary);
	firstRefSlider.setRange(0, 300, 1);
	firstRefSlider.setTextBoxStyle(Slider::TextBoxBelow, true, getWidth(), 30);
	firstRefSlider.setPopupDisplayEnabled(true, false, this);
	firstRefSlider.setTextValueSuffix(" ms");
	firstRefSlider.setValue(100);
	addAndMakeVisible(&firstRefSlider);
	firstRefSlider.addListener(this);

	lateralAmplitudeDifferenceSlider.setSliderStyle(Slider::Rotary);
	lateralAmplitudeDifferenceSlider.setRange(0.0, 1.0, 0.01);
	lateralAmplitudeDifferenceSlider.setTextBoxStyle(Slider::TextBoxBelow, true, getWidth(), 30);
	lateralAmplitudeDifferenceSlider.setPopupDisplayEnabled(true, false, this);
	//lateralAmplitudeDifferenceSlider.setTextValueSuffix(" ");
	lateralAmplitudeDifferenceSlider.setValue(0.0);
	addAndMakeVisible(&lateralAmplitudeDifferenceSlider);
	lateralAmplitudeDifferenceSlider.addListener(this);
	

	addAndMakeVisible(&filtersChoose);
	filtersChoose.addItem("Constant filters", 1);
	filtersChoose.addItem("Delay depentant filters", 2);

	filtersChoose.onChange = [this] { filtersChange(); };
	filtersChoose.setSelectedId(2);


	///////////////TOGGLES//////////////////////

	//noiseToggle.setButtonText("Noise");
	
	//ITD_toggle.setButtonText("ITD");
	ITD_toggle.setToggleState(true, true);
	ITD_toggle.onClick = [this] {Circular_attemptAudioProcessorEditor::onStateSwitchITD(); };
	addAndMakeVisible(&ITD_toggle);
	
	//ILD_toggle.setButtonText("ILD");
	ILD_toggle.setToggleState(false, false);
	ILD_toggle.onClick = [this] {Circular_attemptAudioProcessorEditor::onStateSwitchILD(); };
	addAndMakeVisible(&ILD_toggle);

	noiseToggle.setToggleState(false, false);
	noiseToggle.onClick = [this] {Circular_attemptAudioProcessorEditor::onStateSwitchNoise(); };
	addAndMakeVisible(&noiseToggle);

	///////////////TOGGLES-END////////////////////


	///////////////LABELS/////////////////////////

	addAndMakeVisible(wetDryLabel);
	wetDryLabel.setText("Wet/Dry", dontSendNotification);
	wetDryLabel.addListener(this);
	wetDryLabel.attachToComponent(&trueWetDry, false);

	addAndMakeVisible(reverbLengthLabel);
	reverbLengthLabel.setText("Length", dontSendNotification);
	reverbLengthLabel.addListener(this);
	reverbLengthLabel.attachToComponent(&reverbSizeSlider, false);

	addAndMakeVisible(firstRefTimeLabel);
	firstRefTimeLabel.setText("First reflection time", dontSendNotification);
	firstRefTimeLabel.addListener(this);
	firstRefTimeLabel.attachToComponent(&firstRefSlider, false);

	addAndMakeVisible(lateralRefsLabel);
	lateralRefsLabel.setText("First ref. amp. diff.", dontSendNotification);
	lateralRefsLabel.addListener(this);
	lateralRefsLabel.attachToComponent(&lateralAmplitudeDifferenceSlider, false);
	////END-LABELS////

	/*addAndMakeVisible(wetDryLabelValues);
	wetDryLabelValues.setText("Wet/Dry", dontSendNotification);
	wetDryLabelValues.addListener(this);

	addAndMakeVisible(reverbLengthLabelValues);
	reverbLengthLabelValues.setText("Length", dontSendNotification);
	reverbLengthLabelValues.addListener(this);

	addAndMakeVisible(lateralRefsLabelValues);
	lateralRefsLabelValues.setText("First reflection time", dontSendNotification);
	lateralRefsLabelValues.addListener(this);

	addAndMakeVisible(firstRefTimeLabelValues);
	firstRefTimeLabelValues.setText("First ref. channel amp.", dontSendNotification);
	firstRefTimeLabelValues.addListener(this);*/
	

	addAndMakeVisible(ITD_label);
	ITD_label.setText("ITD", dontSendNotification);
	ITD_label.addListener(this);

	addAndMakeVisible(ILD_label);
	ILD_label.setText("ILD", dontSendNotification);
	ILD_label.addListener(this);

	addAndMakeVisible(noiseLabel);
	noiseLabel.setText("Noise", dontSendNotification);
	noiseLabel.addListener(this);

	//Label ITD_label, ILD_label, noiseLabel;
	///////////////LABELS-END/////////////////////
	
	//directSoundCutoffSlider.setSliderStyle(Slider::Rotary);
	//directSoundCutoffSlider.setRange(20, 20000, 1);
	////directSoundCutoffSlider.setSkewFactorFromMidPoint(500);
	//directSoundCutoffSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 1);
	//directSoundCutoffSlider.setPopupDisplayEnabled(true, false, this);
	//directSoundCutoffSlider.setTextValueSuffix(" ");
	//directSoundCutoffSlider.setValue(20000);
	//addAndMakeVisible(&directSoundCutoffSlider);
	//directSoundCutoffSlider.addListener(this);

}

void Circular_attemptAudioProcessorEditor::labelTextChanged(Label * labelThatHasChanged)
{
	auto asd = firstRefSlider.getValue();
	//firstRefTimeLabel.setText((std::string)(firstRefSlider.getValue()));

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
	/*switch (noiseToggle.getState())
	{
		case 0: processor.reverbEngine.noiseOn = 0;
		case 1: processor.reverbEngine.noiseOn = 1;
		default: break;
	}*/
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
	////wetDrySlider.setBounds(0, 0, 100, 100);
	//ILDwetSlider.setBounds(100, 0, 100, 100);
	//noiseIntensitySlider.setBounds(100, 400, 100, 100);
	//directSoundCutoffSlider.setBounds(400, 0, 100, 100);
	//ITDslider.setBounds(400, 0, 100, 100);

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
	wetDryLabel.setBounds(140, 30, getWidth(), 30);
	reverbLengthLabel.setBounds(340, 30, getWidth(), 30);
	firstRefTimeLabel.setBounds(35, 230, getWidth(), 30);
	lateralRefsLabel.setBounds(235, 230, getWidth(), 30);

	/*wetDryLabelValues.setBounds(70, 140, 100, 30);
	reverbLengthLabelValues.setBounds(270, 140, 100, 30);
	lateralRefsLabelValues.setBounds(40, 340, 140, 30);
	firstRefTimeLabelValues.setBounds(240, 340, 150, 30);*/

	ITD_label.setBounds(405, 230, 50, 30);
	ILD_label.setBounds(475, 230, 50, 30);
	noiseLabel.setBounds(540, 230, 50, 30);
	//////////


	//Label ITD_label, ILD_label, noiseLabel;
	//Label wetDryLabelValues, reverbLengthLabelValues, lateralRefsLabelValues, firstRefTimeLabelValues;
	//Label wetDryLabel, reverbLengthLabel, firstRefTimeLabel, lateralRefsLabel;
}

void Circular_attemptAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
	processor.reverbEngine.mWetDry = trueWetDry.getValue();
	processor.reverbEngine.reverbSize = reverbSizeSlider.getValue();
	processor.reverbEngine.noiseIntensity = noiseIntensitySlider.getValue();
	processor.reverbEngine.firstRefTime = firstRefSlider.getValue();
	processor.reverbEngine.lateralAmplitudeDifference = lateralAmplitudeDifferenceSlider.getValue();

}
