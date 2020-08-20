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
    setSize (400, 200);

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
	trueWetDry.setTextBoxStyle(Slider::NoTextBox, false, 0, 1);
	trueWetDry.setPopupDisplayEnabled(true, false, this);
	trueWetDry.setTextValueSuffix(" ");
	trueWetDry.setValue(1.0f);
	addAndMakeVisible(&trueWetDry);
	trueWetDry.addListener(this);

	reverbSizeSlider.setSliderStyle(Slider::Rotary);
	reverbSizeSlider.setRange(0, numberDelayLines, 1);
	reverbSizeSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 1);
	reverbSizeSlider.setPopupDisplayEnabled(true, false, this);
	reverbSizeSlider.setTextValueSuffix(" ");
	reverbSizeSlider.setValue(numberDelayLines);
	addAndMakeVisible(&reverbSizeSlider);
	reverbSizeSlider.addListener(this);

	
	noiseIntensitySlider.setSliderStyle(Slider::Rotary);
	noiseIntensitySlider.setRange(0.0f, 2.0f, 0.01f);
	noiseIntensitySlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 1);
	noiseIntensitySlider.setPopupDisplayEnabled(true, false, this);
	noiseIntensitySlider.setTextValueSuffix(" ");
	noiseIntensitySlider.setValue(2.0f);
	addAndMakeVisible(&noiseIntensitySlider);
	noiseIntensitySlider.addListener(this);

	firstRefSlider.setSliderStyle(Slider::Rotary);
	firstRefSlider.setRange(0, 400, 1);
	firstRefSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 1);
	firstRefSlider.setPopupDisplayEnabled(true, false, this);
	firstRefSlider.setTextValueSuffix(" ");
	firstRefSlider.setValue(100);
	addAndMakeVisible(&firstRefSlider);
	firstRefSlider.addListener(this);

	lateralAmplitudeDifferenceSlider.setSliderStyle(Slider::Rotary);
	lateralAmplitudeDifferenceSlider.setRange(0.0, 1.0, 0.01);
	lateralAmplitudeDifferenceSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 1);
	lateralAmplitudeDifferenceSlider.setPopupDisplayEnabled(true, false, this);
	lateralAmplitudeDifferenceSlider.setTextValueSuffix(" ");
	lateralAmplitudeDifferenceSlider.setValue(0.0);
	addAndMakeVisible(&lateralAmplitudeDifferenceSlider);
	lateralAmplitudeDifferenceSlider.addListener(this);
	

	addAndMakeVisible(&filtersChoose);
	filtersChoose.addItem("Constant Filters", 1);
	filtersChoose.addItem("Frequency depentant filters", 2);

	filtersChoose.onChange = [this] { filtersChange(); };
	filtersChoose.setSelectedId(1);
	//filtersChoose.addListener(this);
	
	//directSoundCutoffSlider.setSliderStyle(Slider::Rotary);
	//directSoundCutoffSlider.setRange(20, 20000, 1);
	////directSoundCutoffSlider.setSkewFactorFromMidPoint(500);
	//directSoundCutoffSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 1);
	//directSoundCutoffSlider.setPopupDisplayEnabled(true, false, this);
	//directSoundCutoffSlider.setTextValueSuffix(" ");
	//directSoundCutoffSlider.setValue(20000);
	//addAndMakeVisible(&directSoundCutoffSlider);
	//directSoundCutoffSlider.addListener(this);


	/*ITDslider.setSliderStyle(Slider::Rotary);
	ITDslider.setRange(1, 20, 1);
	ITDslider.setTextBoxStyle(Slider::NoTextBox, false, 0, 1);
	ITDslider.setPopupDisplayEnabled(true, false, this);
	ITDslider.setTextValueSuffix(" ");
	ITDslider.setValue(20);
	addAndMakeVisible(&ITDslider);
	ITDslider.addListener(this);*/
	//wetDrySlider

	//reverbSize

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
	trueWetDry.setBounds(100, 0, 100, 100);
	reverbSizeSlider.setBounds(200, 0, 100, 100);
	noiseIntensitySlider.setBounds(50, 100, 100, 100);
	firstRefSlider.setBounds(150, 100, 100, 100);

	lateralAmplitudeDifferenceSlider.setBounds(250, 100, 100, 100);
	//directSoundCutoffSlider.setBounds(400, 0, 100, 100);
	//ITDslider.setBounds(400, 0, 100, 100);
	filtersChoose.setBounds(10, 40, getWidth() - 20, 20);
}

void Circular_attemptAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
	//processor.reverbLength = reverbLengthSlider.getValue();
	////processor.reverbSize = reverbSizeSlider.getValue();
	//processor.firstRefTime = reverbSizeSlider.getValue();
	//processor.reverbEngine.wetDry = wetDrySlider.getValue();
	//processor.reverbEngine.reflectionAmplitude = 1 - wetDrySlider.getValue();

	//processor.reverbEngine.ILDwet = ILDwetSlider.getValue();

	processor.reverbEngine.mWetDry = trueWetDry.getValue();

	processor.reverbEngine.reverbSize = reverbSizeSlider.getValue();

	processor.reverbEngine.noiseIntensity = noiseIntensitySlider.getValue();

	processor.reverbEngine.firstRefTime = firstRefSlider.getValue();

	processor.reverbEngine.lateralAmplitudeDifference = lateralAmplitudeDifferenceSlider.getValue();

	//processor.reverbEngine.firstRefTime = firstRefSlider.getValue();

	//processor.reverbEngine.directSoundCutoff = directSoundCutoffSlider.getValue();

	//processor.reverbEngine.spatialMaker.ITDasd = ITDslider.getValue();
}
