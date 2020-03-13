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

	sampleRate_ = sampleRate;

	const int numInputChannels = getNumInputChannels();
	const int delayBufferSize = 5 * (sampleRate); // 'widzimy' dwie sekundy sygna³u w ty³ + troszkê wiêcej (+ 2* iloœæ próbek w buforze)

	delayBuffer.setSize(getNumInputChannels(), delayBufferSize);
	delayBuffer.clear();

	delayTimesNumber = 20;
	delayTimesArray = delayTimes.getDelayTimes(delayTimesNumber);
	//delayTimesNumber += 1;

	dsp::ProcessSpec spec;
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = getTotalNumOutputChannels();

	filtersNumber = delayTimesNumber;
	lowBorderFilterFrequency = 500;
	highBorderFilterFrequency = 2000;
	for (int filter = 0; filter < filtersNumber; ++filter)
	{
		if (filtersNumber <= 20)
			filterCutoffFrequencies.push_back(filterGenerator.getFilterCutoffFrequency(lowBorderFilterFrequency, highBorderFilterFrequency));
	}

	for (int filter = 0; filter < filtersNumber; ++filter)
	{
		lowPassFilter[filter].prepare(spec);
		lowPassFilter[filter].reset();
		*(lowPassFilter[filter]).state = *dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, filterCutoffFrequencies[filter], 1.0f);
	}
	
	for (int filter = 0; filter < filtersNumber; ++filter)
	{
		ITDCoefficients.push_back(getITDTime());
	}
}

int Circular_attemptAudioProcessor::getITDTime()
{
	return Random::getSystemRandom().nextInt(Range<int>(-8, 8));
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

	dsp::AudioBlock<float> block(buffer);

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	const int bufferLength = buffer.getNumSamples();
	const int delayBufferLength = delayBuffer.getNumSamples();
	
	const float* bufferDataL = buffer.getReadPointer(leftChannel);
	const float* bufferDataR = buffer.getReadPointer(rightChannel);
	const float* delayBufferDataL = delayBuffer.getReadPointer(leftChannel);
	const float* delayBufferDataR = delayBuffer.getReadPointer(rightChannel);

	float* dryBufferL = buffer.getWritePointer(leftChannel);
	float* dryBufferR = buffer.getWritePointer(rightChannel);

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
		const float* bufferData = buffer.getReadPointer(channel);
		const float* delayBufferData = delayBuffer.getReadPointer(channel);
		copyBufferToDelayBuffer(channel, bufferData, delayBufferData, bufferLength, delayBufferLength);
    }

	for (int filter = 0; filter < filtersNumber; ++filter)
	{
		copyBackToCurrentBuffer(buffer, leftChannel, bufferDataL, delayBufferDataL, bufferLength, delayBufferLength, delayTimesArray[filter]);
		copyBackToCurrentBuffer(buffer, rightChannel, bufferDataR, delayBufferDataR, bufferLength, delayBufferLength, delayTimesArray[filter] + ITDCoefficients[filter]);

		lowPassFilter[filter].process(dsp::ProcessContextReplacing<float>(block));
		addDelayWithCurrentBuffer(leftChannel, bufferLength, delayBufferLength, dryBufferL, delayTimesNumber);
		addDelayWithCurrentBuffer(rightChannel, bufferLength, delayBufferLength, dryBufferR, delayTimesNumber);
	}
	
	
	/*copyBackToCurrentBuffer(buffer, 0, bufferDataL, delayBufferDataL, bufferLength, delayBufferLength, 150);
	copyBackToCurrentBuffer(buffer, 1, bufferDataR, delayBufferDataR, bufferLength, delayBufferLength, 150.5);

	lowPassFilter[1].process(dsp::ProcessContextReplacing<float>(block));
	addDelayWithCurrentBuffer(0, bufferLength, delayBufferLength, dryBufferL, delayTimesNumber);
	addDelayWithCurrentBuffer(1, bufferLength, delayBufferLength, dryBufferR, delayTimesNumber);



	copyBackToCurrentBuffer(buffer, 0, bufferDataL, delayBufferDataL, bufferLength, delayBufferLength, 320);
	copyBackToCurrentBuffer(buffer, 1, bufferDataR, delayBufferDataR, bufferLength, delayBufferLength, 319.5);

	lowPassFilter[2].process(dsp::ProcessContextReplacing<float>(block));
	addDelayWithCurrentBuffer(0, bufferLength, delayBufferLength, dryBufferL, delayTimesNumber);
	addDelayWithCurrentBuffer(1, bufferLength, delayBufferLength, dryBufferR, delayTimesNumber);
*/


	copyBackToCurrentBuffer(buffer, 0, bufferDataL, delayBufferDataL, bufferLength, delayBufferLength, 0);
	copyBackToCurrentBuffer(buffer, 1, bufferDataR, delayBufferDataR, bufferLength, delayBufferLength, 0);

	addDelayWithCurrentBuffer(0, bufferLength, delayBufferLength, dryBufferL, delayTimesNumber);
	addDelayWithCurrentBuffer(1, bufferLength, delayBufferLength, dryBufferR, delayTimesNumber);

	/////////////

	bufferWritePosition += bufferLength; //przesuwamy pozycjê czyli miejsce do którego wklejamy nasz buffer
									//czyli je¿eli bufor ma 512 próbek to kolejmy wklejamy w miejsce od indeksu 513
	bufferWritePosition = bufferWritePosition % delayBufferLength;	//je¿eli pozycja wpisania wykracza poza d³ugoœæ bufora to znowu idzie na pocz¹tek:
															//np. bufor opóŸniaj¹cy to 2048. pozycja wpisania to 2048, dziêki modulo pozycja idzie znów na 
															//pocz¹tek czyli na 0, potem 1
															//mWritePosition to pole klasy - bêdziemy tego indeksu u¿ywaæ te¿ przy czytaniu z bufora


}

void Circular_attemptAudioProcessor::copyBufferToDelayBuffer(int channel, const float* bufferData, const float* delayBufferData,
																const int bufferLength, const int delayBufferLength)
{
	//kopiujemy dane z naszego buffera do delay buffera

	//sprawdzenie, czy dlugosc bufora opozniajacego jest wieksza niz d³ugoœæ bufora + pozycja. czyli 
	if (delayBufferLength > bufferLength + bufferWritePosition)	//mo¿e byc taka sytuacja - np bufor ma dlugosc 512, bufor delay 2048, pozycja to 1537 (3* 512)
															//czyli 1537 + 512 > 2038 (jest równe 2049)? cos takiego, nie rozumiem do koñca
															//wiêc musimy w tej sytuacji przesuwamy na dlugosc bufora opozniajacego minus pozycja 
	{
		delayBuffer.copyFromWithRamp(channel, bufferWritePosition, bufferData, bufferLength, 1, 1);
	}
	else
	{
		const int bufferRemaning = delayBufferLength - bufferWritePosition; //index 
		delayBuffer.copyFromWithRamp(channel, bufferWritePosition, bufferData, bufferRemaning, 1, 1);

		delayBuffer.copyFromWithRamp(channel, 0, bufferData, bufferLength - bufferRemaning, 1, 1);	//to co zosta³o wklejamy na pocz¹tek. 
																										//czyli to co zosta³o odciête (ca³y bufor to 
																										//bufferRemaining + ta reszta któr¹ tu doklejamy
	}
}

void Circular_attemptAudioProcessor::getFirstReflection(AudioBuffer<float>& buffer, int channel, const float* bufferData, const float* delayBufferData,
	const int bufferLength, const int delayBufferLength, int delayTime)
{
	//int delayTime = 500;
	const int readPosition = static_cast<int>(delayBufferLength + bufferWritePosition - (sampleRate_ * delayTime / 1000)) % delayBufferLength;

	if (delayBufferLength > bufferLength + readPosition)				
																		
																		
	{
		buffer.copyFromWithRamp(channel, 0, delayBufferData + readPosition, bufferLength, 1, 1);
	}
	else																
																		
	{
		const int bufferRemaining = delayBufferLength - readPosition;	
		buffer.copyFromWithRamp(channel, 0, delayBufferData + readPosition, bufferRemaining, 1, 1);
		buffer.copyFromWithRamp(channel, bufferRemaining, delayBufferData, bufferLength - bufferRemaining, 1, 1);
	}
}


void Circular_attemptAudioProcessor::copyBackToCurrentBuffer(AudioBuffer<float>& buffer, int channel, const float* bufferData, const float* delayBufferData,
																				const int bufferLength, const int delayBufferLength, int delayTime)
{
	//int delayTime = 500;
	const int bufferReadPosition = static_cast<int>(delayBufferLength + bufferWritePosition - (sampleRate_ * delayTime / 1000)) % delayBufferLength; 
																									//ile próbek dalej bêdziemy 'wklejaæ' sygna³. 
																									//wiêc musi byæ to int. Modulo znów ¿eby 'zawin¹æ' sygna³
																									//w tym buforze - indeks idzie na pocz¹tek
																									//ten static_cast<int> to po prostu rzutowanie na int, tak 
																									//samo jakby napisaæ (int)zmienna. ¿eby upewniæ siê, ¿e 
																									//liczymy modulo z inta (bo to index wiêc musi byæ intem)
																									//jest plus mWritePosition ¿ebyœmy siê przesuwali po prostu 
																									//i czytali za ka¿dym razem mWritePosition dalej (bo wczytujemy 
																									//te¿ przesuwaj¹c siê o taki krok)
	
	
	

	if (delayBufferLength > bufferLength + bufferReadPosition)			//ten if to analogicznie jak przy czytaniu - dopóku jest wystarczaj¹ca iloœæ próbek 
																		//w buforze opóŸniaj¹cym ¿eby wzi¹æ stamt¹d iloœæ próbek równ¹ d³ugoœci naszego bufora,
																		//to dodajemy kawa³ek o d³ugoœci naszego bufora (z przesz³oœci) do naszego bufora
	{
		buffer.copyFromWithRamp(channel, 0, delayBufferData + bufferReadPosition, bufferLength, 1, 1);
	}
	else																//je¿eli nie mamy tyle próbek (bufor opóŸniaj¹cy siê 'koñczy') to bierzemy kawa³ek z koñca
																		//i resztê z pocz¹tku 
	{
		const int bufferRemaining = delayBufferLength - bufferReadPosition;		//ile wartoœci zosta³o w delay bufferze
		buffer.copyFromWithRamp(channel, 0, delayBufferData + bufferReadPosition, bufferRemaining, 1, 1);
		buffer.copyFromWithRamp(channel, bufferRemaining, delayBufferData, bufferLength - bufferRemaining, 1, 1);
	}
	//buffer.applyGain(0.9);
}

void Circular_attemptAudioProcessor::addDelayWithCurrentBuffer(int channel,const int bufferLength, 
																	const int delayBufferLength,  float* bufferData, int delayTimesNumber)
{

	//const float* dryRead = dryBuffer.getReadPointer()
	float amplitudeMultiplier = 0.95 / (delayTimesNumber);
	//float amplitudeMultiplier = 0.15;
	if (delayBufferLength > bufferLength + bufferWritePosition)																
	{
		
		delayBuffer.addFromWithRamp(channel, bufferWritePosition, bufferData, bufferLength, amplitudeMultiplier, amplitudeMultiplier);		//ostatnie 2 argumenty - jak szybko zanika 
																														// - mno¿enie amplitudy z ka¿dym odbiciem

	}
	else
	{
		const int bufferRemaining = delayBufferLength - bufferWritePosition;
		delayBuffer.addFromWithRamp(channel, bufferWritePosition, bufferData, bufferRemaining, amplitudeMultiplier, amplitudeMultiplier);
		delayBuffer.addFromWithRamp(channel, 0, bufferData, bufferLength - bufferRemaining, amplitudeMultiplier, amplitudeMultiplier);

	}
	//mDelayBuffer.applyGain(0.7);
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
