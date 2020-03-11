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
                       ),tree(*this, nullptr), lowPassFilter(dsp::IIR::Coefficients<float>::makeLowPass(44100, 20000.0f, 0.1))
#endif
{
	NormalisableRange<float> cutoffRange(20.0f, 20000.0f);
	NormalisableRange<float> resRange(1.0f, 5.0f);
	//NormalisableRange<float> resRange(1.0f, 5.0f);

	tree.createAndAddParameter("cutoff", "Cutoff", "cutoff", cutoffRange, 600.0f, nullptr, nullptr);
	tree.createAndAddParameter("resonance", "Resonance", "resonance", resRange, 1.0f, nullptr, nullptr);


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
	//const int delayBufferSize = 500 * samplesPerBlock;

	delayBuffer.setSize(getNumInputChannels(), delayBufferSize);
	delayBuffer.clear();
	
	//secondBuffer.setSize(getNumInputChannels(), delayBufferSie);

	delayTimesNumber = 12;
	delayTimesArray = delayTimes.getDelayTimes(delayTimesNumber);
	delayTimesNumber += 4;

	//filter = new IIRFilter(IIRCoefficients::makeHighPass(sampleRate, 1000, 1.0)); // in prepareToPlay

	dsp::ProcessSpec spec;
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = getTotalNumOutputChannels();

	lowPassFilter.prepare(spec);
	lowPassFilter.reset();

	//noiseCoeffsArray = noiseCoeffs.getNoiseCoefficients(samplesPerBlock);
	//delayTimesArray.clear();
	//int firstRefTime = 600;
	/*delayTimesArray.push_back(89);
	delayTimesArray.push_back(200);
	delayTimesArray.push_back(320);
	delayTimesArray.push_back(405);
	delayTimesArray.push_back(30);
	delayTimesArray.push_back(60);
	delayTimesArray.push_back(250);
	delayTimesArray.push_back(110);*/
	//delayTimesArray.push_back();
	//delayTimesArray.push_back(10);
	
	//delayTimesArray.push_back(197);
	//delayTimesArray[0] = 20;
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

	const int bufferLength = buffer.getNumSamples();
	const int delayBufferLength = delayBuffer.getNumSamples();
	

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
		const float* bufferData = buffer.getReadPointer(channel);
		const float* delayBufferData = delayBuffer.getReadPointer(channel);
		float* delayBufferDataWrite = delayBuffer.getWritePointer(channel);
		
		float* dryBuffer = buffer.getWritePointer(channel);

		//musimy kopiowaæ nasz buffer i wklejaæ go do delay buffera
		//czytanie z naszego bufora:
		copyBufferToDelayBuffer(channel, bufferData, delayBufferData, bufferLength, delayBufferLength);

		////w tym miejscu powinny być chyba operacje w celu uzyskania pierwszych odbić - jakieś opóźnienie tego co występi dalej,
		////dodanie zer czy cokolwiek, jakieś poczekanie na to co jest dalej
		auto* channelData = buffer.getWritePointer(channel, 0);


		for (int i = 0; i < delayTimesArray.size(); ++i)
		{
			copyBackToCurrentBuffer(buffer, channel, bufferData, delayBufferData, bufferLength, delayBufferLength, firstRefTime + delayTimesArray[i]);
			//może w tym momencie filtrować - dzięki temu będę mogł filtrować odbicie od konkretnej ściany 
			//czyli nawet lepiej że mam tą pętle zamiast losowania w processBlocku - bo z każdym buforem odbicie 
			//czyli w tym miejscu filtrować zależnie od wartości wylosowanego czasu opóźnienia
			//tak, chyba dokładnie w tym miejscu - przed dodaniem do sygnału czystego
			//ps. co jeżeli w tym miejscu modyfikować amplitudę? 
			//!!! może w zależności od tego który obrót pętli - ustawiać amplitudę?? wtedy możnaby to zrobić faktycznie w sposób 
			//wykładniczy!!!
			addDelayWithCurrentBuffer(channel, bufferLength, delayBufferLength, dryBuffer, delayTimesNumber);

			
			/*if (i > delayTimesArray.size() - delayTimesArray.size()/2)
				for (int smp = 0; smp < bufferLength; ++smp)
					channelData[smp] += (50 * noiseCoeffsArray[smp]);*/
		}


		copyBackToCurrentBuffer(buffer, channel, bufferData, delayBufferData, bufferLength, delayBufferLength, firstRefTime);
		addDelayWithCurrentBuffer(channel, bufferLength, delayBufferLength, dryBuffer, delayTimesNumber);
		/*for (int smp = 0; smp < bufferLength; ++smp)
		{
			channelData[smp] += (50 * noiseCoeffsArray[smp]);
		}*/
		copyBackToCurrentBuffer(buffer, channel, bufferData, delayBufferData, bufferLength, delayBufferLength, 10);
		addDelayWithCurrentBuffer(channel, bufferLength, delayBufferLength, dryBuffer, delayTimesNumber);
		//lowPassFilter.process(dsp::ProcessContextReplacing<float>(block));

		
		//updateFilter();
		
		
		


		
		//normalizacja amplitudy
		/*for (int smp = 0; smp < bufferLength; ++smp)
		{
			channelData[smp] *= 10.0;
		}*/
		
		/*for (auto i = 0; i < 3; i++)
		{
			auto reflectionTime = Random::getSystemRandom().nextInt(Range<int>(40, 300));
			copyBackToCurrentBuffer(buffer, channel, bufferData, delayBufferData, bufferLength, delayBufferLength, reflectionTime);
			addDelayWithCurrentBuffer(channel, bufferLength, delayBufferLength, dryBuffer);
		}*/
		/*copyBackToCurrentBuffer(buffer, channel, bufferData, delayBufferData, bufferLength, delayBufferLength, 30);
		addDelayWithCurrentBuffer(channel, bufferLength, delayBufferLength, dryBuffer);

		copyBackToCurrentBuffer(buffer, channel, bufferData, delayBufferData, bufferLength, delayBufferLength, 400);
		addDelayWithCurrentBuffer(channel, bufferLength, delayBufferLength, dryBuffer);


		copyBackToCurrentBuffer(buffer, channel, bufferData, delayBufferData, bufferLength, delayBufferLength, 50);
		addDelayWithCurrentBuffer(channel, bufferLength, delayBufferLength, dryBuffer);


		copyBackToCurrentBuffer(buffer, channel, bufferData, delayBufferData, bufferLength, delayBufferLength, 340);
		addDelayWithCurrentBuffer(channel, bufferLength, delayBufferLength, dryBuffer);

		copyBackToCurrentBuffer(buffer, channel, bufferData, delayBufferData, bufferLength, delayBufferLength, 98);
		addDelayWithCurrentBuffer(channel, bufferLength, delayBufferLength, dryBuffer);*/
    }

	


	bufferWritePosition += bufferLength; //przesuwamy pozycjê czyli miejsce do którego wklejamy nasz buffer
									//czyli je¿eli bufor ma 512 próbek to kolejmy wklejamy w miejsce od indeksu 513
	bufferWritePosition = bufferWritePosition % delayBufferLength;	//je¿eli pozycja wpisania wykracza poza d³ugoœæ bufora to znowu idzie na pocz¹tek:
															//np. bufor opóŸniaj¹cy to 2048. pozycja wpisania to 2048, dziêki modulo pozycja idzie znów na 
															//pocz¹tek czyli na 0, potem 1
															//mWritePosition to pole klasy - bêdziemy tego indeksu u¿ywaæ te¿ przy czytaniu z bufora


	dsp::AudioBlock<float> block(buffer);
	*lowPassFilter.state = *dsp::IIR::Coefficients<float>::makeLowPass(sampleRate_, 500.0f, 1.0f);
	lowPassFilter.process(dsp::ProcessContextReplacing<float>(block));

}

void Circular_attemptAudioProcessor::updateFilter()
{
	float freq = *tree.getRawParameterValue("cutoff");
	float res = *tree.getRawParameterValue("resonance");

	*lowPassFilter.state = *dsp::IIR::Coefficients<float>::makeLowPass(sampleRate_, 8000.0f, 2.0f);
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
	float amplitudeMultiplier = 0.7 / (delayTimesNumber + 1) * reverbLength;
	//float amplitudeMultiplier = 0.042;
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
