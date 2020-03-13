/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DelayTimesGenerator.h"

//==============================================================================
/**
*/
class Circular_attemptAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Circular_attemptAudioProcessor();
    ~Circular_attemptAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

	void updateFilter();

	void copyBufferToDelayBuffer(int channel, const float * bufferData, const float * delayBufferData, const int bufferLength, const int delayBufferLength);

	void copyBackToCurrentBuffer(AudioBuffer<float>& buffer, int channel, const float * bufferData, const float * delayBufferData,
																				const int bufferLength, const int delayBufferLength, int delayTime);

	void addDelayWithCurrentBuffer(int channel, const int bufferLength, const int delayBufferLength,  float* dry, int delayTimesNumber);

	void getFirstReflection(AudioBuffer<float>& buffer, int channel, const float* bufferData, const float* delayBufferData,
																				const int bufferLength, const int delayBufferLength, int delayTime);

	float reverbLength = 0.5;
	int reverbSize = 0;
	int firstRefTime = 0;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	
	AudioProcessorValueTreeState tree;

private:
	AudioBuffer<float> delayBuffer, secondBuffer, fadeBuffer;
	int bufferWritePosition{ 0 };
	int sampleRate_ = 0;
	DelayTimesGenerator delayTimes;
	DelayTimesGenerator noiseCoeffs;
	std::vector<int> delayTimesArray;
	std::vector<float> noiseCoeffsArray;
	int delayTimesNumber;

	dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> lowPassFilter[10];

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Circular_attemptAudioProcessor)
};
