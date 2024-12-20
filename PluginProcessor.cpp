/*
  ==============================================================================

//Date: 2020
//Author: Juce + Adam Korytowski

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Circular_attemptAudioProcessor::Circular_attemptAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )/*,tree(*this, nullptr)*/
#endif
{

}

Circular_attemptAudioProcessor::~Circular_attemptAudioProcessor()
{
}

//==============================================================================
const String Circular_attemptAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Circular_attemptAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Circular_attemptAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Circular_attemptAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Circular_attemptAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Circular_attemptAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Circular_attemptAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Circular_attemptAudioProcessor::setCurrentProgram (int index)
{
}

const String Circular_attemptAudioProcessor::getProgramName (int index)
{
    return {};
}

void Circular_attemptAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Circular_attemptAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	reverbEngine.numInputChannels = getTotalNumInputChannels();
	reverbEngine.numOutputChannels = getTotalNumOutputChannels();
	reverbEngine.prepare(sampleRate, samplesPerBlock, getNumOutputChannels());
}

void Circular_attemptAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up anyf
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Circular_attemptAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Circular_attemptAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{

    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	reverbEngine.process(buffer);
}



//==============================================================================
bool Circular_attemptAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Circular_attemptAudioProcessor::createEditor()
{
    return new Circular_attemptAudioProcessorEditor (*this);
}

//==============================================================================
void Circular_attemptAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Circular_attemptAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Circular_attemptAudioProcessor();
}
