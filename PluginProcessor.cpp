/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

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
	//NormalisableRange<float> cutoffRange(20.0f, 20000.0f);
	//NormalisableRange<float> resRange(1.0f, 5.0f);
	////NormalisableRange<float> resRange(1.0f, 5.0f);

	//tree.createAndAddParameter("cutoff", "Cutoff", "cutoff", cutoffRange, 600.0f, nullptr, nullptr);
	//tree.createAndAddParameter("resonance", "Resonance", "resonance", resRange, 1.0f, nullptr, nullptr);


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

	
	//szum:
	//for (int time = 408; time < 408 + 40; time += 20)
	//{
	//	float* bufferWriteL = buffer.getWritePointer(leftChannel);
	//	float* bufferWriteR = buffer.getWritePointer(rightChannel);
	//	
	//	copyBackToCurrentBuffer(buffer, leftChannel, bufferDataL, delayBufferDataL, bufferLength, delayBufferLength, time);
	//	copyBackToCurrentBuffer(buffer, rightChannel, bufferDataR, delayBufferDataR, bufferLength, delayBufferLength, time);
	//	
	//	AudioBuffer<float>noiseBuffer;
	//	noiseBuffer.setSize(2, bufferLength);
	//	noiseBuffer.clear();
	//	for (int sample = 0; sample < bufferLength; ++sample)
	//	{
	//		noiseBuffer.addSample(0, sample, Random::getSystemRandom().nextFloat() / 5.0f);
	//	}
	//	float* noiseBufferData = noiseBuffer.getWritePointer(0);
	//	//buffer.addFrom(0, 0, noiseBufferData, bufferLength);
	//	for (int sample = 0; sample < bufferLength; ++sample)
	//	{
	//		//noiseBufferData[sample] *= std::pow(0.5, sample);
	//		//buffer.addSample(0, bufferDataL[sample], noiseBufferData[sample]);
	//		bufferWriteL[sample] *= noiseBufferData[sample];
	//		bufferWriteR[sample] *= noiseBufferData[sample];
	//	}
	///////////////////////////
		//addDelayWithCurrentBuffer(leftChannel, bufferLength, delayBufferLength, dryBufferL, 3);
		//addDelayWithCurrentBuffer(rightChannel, bufferLength, delayBufferLength, dryBufferR, 3);	
	//}
	

	/////////////
	//AudioBuffer<float>noiseBuffer;
	//noiseBuffer.setSize(2, bufferLength);
	//noiseBuffer.clear();
	//for (int sample = 0; sample < bufferLength; ++sample)
	//{
	//	noiseBuffer.addSample(0, sample, Random::getSystemRandom().nextFloat() / 10.0f);
	//}
	//float* noiseBufferData = noiseBuffer.getWritePointer(0);
	////buffer.addFrom(0, 0, noiseBufferData, bufferLength);
	//float* bufferWrite = buffer.getWritePointer(0);
	//for (int sample = 0; sample < bufferLength; ++sample)
	//{
	//	//buffer.addSample(0, bufferDataL[sample], noiseBufferData[sample]);
	//	bufferWrite[sample] *= noiseBufferData[sample];
	//}
	

	//////////////////////tutaj jest sposób dodawania/mnożenia każdej próbki przez szum//////////////////////
	//AudioBuffer<float>noiseBuffer;
	//noiseBuffer.setSize(2, bufferLength);
	//noiseBuffer.clear();
	//for (int sample = 0; sample < bufferLength; ++sample)
	//{
	//	noiseBuffer.addSample(0, sample, Random::getSystemRandom().nextFloat() / 10.0f);
	//}
	//float* noiseBufferData = noiseBuffer.getWritePointer(0);
	////buffer.addFrom(0, 0, noiseBufferData, bufferLength);
	//float* bufferWrite = buffer.getWritePointer(0);
	//for (int sample = 0; sample < bufferLength; ++sample)
	//{
	//	//buffer.addSample(0, bufferDataL[sample], noiseBufferData[sample]);
	//	bufferWrite[sample] *= noiseBufferData[sample];
	//}
	

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
