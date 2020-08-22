/*
  ==============================================================================

    ReverbEngine.h
    Created: 28 Mar 2020 12:27:41pm
    Author:  Adam

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "DelayTimesGenerator.h"
#include "FilterGenerator.h"
#include "SpatialMaker.h"
#include "LateReverb.h"

class ReverbEngine: public ReverbElement
{
public:
	ReverbEngine() {}

	FilterGenerator filterGenerator;
	SpatialMaker spatialMaker;
	DelayTimesGenerator delayTimes;
	LateReverb lateReverb;

	/*...*/

	std::vector<int> delayTimesArray;
	int delayTimesNumber;
	void process(AudioBuffer<float>&buffer);
	void prepare(double sampleRate, int samplesPerBlock, int numChannels);
	const bool leftChannel{0}, rightChannel{1};
	AudioBuffer<float> delayBuffer;
	int bufferWritePosition{ 0 };
	int sampleRate_ = 0;
	void copyBufferToDelayBuffer(int channel, const float * bufferData, const float * delayBufferData, const int bufferLength, const int delayBufferLength);
	void copyBackToCurrentBuffer(AudioBuffer<float>& buffer, int channel, const float * bufferData, const float * delayBufferData,
																	const int bufferLength, const int delayBufferLength, int delayTime);
	void addDelayWithCurrentBuffer(int channel, const int bufferLength, const int delayBufferLength, const float* dry, float amplitudeMultiplier);

	int numInputChannels;
	int numOutputChannels;
	//float amplitudeEarly, amplitudeLate;
	//float wetDry = 0;
	//float reflectionAmplitude = 0.9;
	//int asd = 0;
	float ILDwet = 0;

	float mWetDry/*{ 0.1 }*/;
	float reverbSize;
	int ITDcoefficient;
	int directSoundCutoff;
	std::vector<AudioBuffer<float>>noiseArray;
	//AudioBuffer<float>asdfg;
	AudioBuffer<float>noiseBuffer;
	std::vector<float>noiseVector;
	float noiseIntensity;
	int firstRefTime = 200;
	float lateralAmplitudeDifference;

	float ILDfactor;

	bool filtersOption{ 0 };
	bool noiseOn{ true };
	bool ITD_on;
	bool ILD_on;

	/*void asd(AudioBuffer<float>& buffer, int channel, const float* bufferData, const float* delayBufferData,
		const int bufferLength, const int delayBufferLength, int delayTime);*/

};