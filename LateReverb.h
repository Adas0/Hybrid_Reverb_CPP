/*
  ==============================================================================

    LateReverb.h
    Created: 29 Mar 2020 11:22:26am
    Author:  Adam

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>
#include "ReverbElement.h"
#include "DelayTimesGenerator.h"

class LateReverb: public ReverbElement, DelayTimesGenerator
{
public:
	LateReverb() {}
	void addLateReverb(std::vector<int>& delayTimes);
	void addLateNoise(AudioBuffer<float>noiseBuffer, AudioBuffer<float>&buffer, float* noiseBufferDataL, float* noiseBufferDataR, 
																							float* bufferWriteL, float* bufferWriteR);
	float generateRandomNoise();
	int lateReverbNumLines;
	AudioBuffer<float> createNoiseBufferArray(int& bufferSize);
	AudioBuffer<float>noiseBuffer;
	//std::vector<int> primeNumbers = { 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71 };
	std::vector<AudioBuffer<float>> noiseBuffers;

};