/*
  ==============================================================================

    ReverbEngine.cpp
    Created: 28 Mar 2020 12:27:48pm
    Author:  Adam

  ==============================================================================
*/

#include "ReverbEngine.h"

void ReverbEngine::prepare(double sampleRate, int samplesPerBlock, int numChannels)
{
	sampleRate_ = sampleRate;
	filterGenerator.prepare(sampleRate, samplesPerBlock, numChannels);
	spatialMaker.prepare();

	const int delayBufferSize = 6 * (sampleRate);
	delayBuffer.setSize(numChannels, delayBufferSize);
	delayBuffer.clear();

	delayTimesNumber = filterGenerator.numberDelayLines;
	delayTimesArray = delayTimes.getDelayTimes(delayTimesNumber);
	/*delayTimesArray[0] = 80;
	delayTimesArray[1] = 120;
	delayTimesArray[2] = 140;*/
}


void ReverbEngine::process(AudioBuffer<float>&buffer)
{
	dsp::AudioBlock<float> block(buffer);

	const int bufferLength = buffer.getNumSamples();
	const int delayBufferLength = delayBuffer.getNumSamples();

	const float* bufferDataL = buffer.getReadPointer(leftChannel);
	const float* bufferDataR = buffer.getReadPointer(rightChannel);
	const float* delayBufferDataL = delayBuffer.getReadPointer(leftChannel);
	const float* delayBufferDataR = delayBuffer.getReadPointer(rightChannel);


	float* dryBufferL = buffer.getWritePointer(leftChannel);
	float* dryBufferR = buffer.getWritePointer(rightChannel);

	for (int channel = 0; channel < numInputChannels; ++channel)
	{
		const float* bufferData = buffer.getReadPointer(channel);
		const float* delayBufferData = delayBuffer.getReadPointer(channel);
		copyBufferToDelayBuffer(channel, bufferData, delayBufferData, bufferLength, delayBufferLength);
	}
	
	for (int filter = 0; filter < filterGenerator.filtersNumber; ++filter)
	{
		copyBackToCurrentBuffer(buffer, leftChannel, bufferDataL, delayBufferDataL, bufferLength, delayBufferLength, delayTimesArray[filter]);
		copyBackToCurrentBuffer(buffer, rightChannel, bufferDataR, delayBufferDataR, bufferLength, delayBufferLength, delayTimesArray[filter] + spatialMaker.ITDCoefficients[filter]);



		filterGenerator.lowPassFilter[filter].process(dsp::ProcessContextReplacing<float>(block));
		addDelayWithCurrentBuffer(leftChannel, bufferLength, delayBufferLength, dryBufferL, delayTimesNumber);
		addDelayWithCurrentBuffer(rightChannel, bufferLength, delayBufferLength, dryBufferR, delayTimesNumber);
	}

	filterGenerator.allPassFilter.process(dsp::ProcessContextReplacing<float>(block));

	copyBackToCurrentBuffer(buffer, leftChannel, bufferDataL, delayBufferDataL, bufferLength, delayBufferLength, 0);
	copyBackToCurrentBuffer(buffer, rightChannel, bufferDataR, delayBufferDataR, bufferLength, delayBufferLength, 0);

	addDelayWithCurrentBuffer(leftChannel, bufferLength, delayBufferLength, dryBufferL, delayTimesNumber);
	addDelayWithCurrentBuffer(rightChannel, bufferLength, delayBufferLength, dryBufferR, delayTimesNumber);


	bufferWritePosition += bufferLength; //przesuwamy pozycj� czyli miejsce do kt�rego wklejamy nasz buffer
								//czyli je�eli bufor ma 512 pr�bek to kolejmy wklejamy w miejsce od indeksu 513
	bufferWritePosition = bufferWritePosition % delayBufferLength;	//je�eli pozycja wpisania wykracza poza d�ugo�� bufora to znowu idzie na pocz�tek:
															//np. bufor op�niaj�cy to 2048. pozycja wpisania to 2048, dzi�ki modulo pozycja idzie zn�w na 
															//pocz�tek czyli na 0, potem 1
															//mWritePosition to pole klasy - b�dziemy tego indeksu u�ywa� te� przy czytaniu z bufora

}


void ReverbEngine::copyBufferToDelayBuffer(int channel, const float* bufferData, const float* delayBufferData,
	const int bufferLength, const int delayBufferLength)
{
	//kopiujemy dane z naszego buffera do delay buffera

	//sprawdzenie, czy dlugosc bufora opozniajacego jest wieksza niz d�ugo�� bufora + pozycja. czyli 
	if (delayBufferLength > bufferLength + bufferWritePosition)	//mo�e byc taka sytuacja - np bufor ma dlugosc 512, bufor delay 2048, pozycja to 1537 (3* 512)
															//czyli 1537 + 512 > 2038 (jest r�wne 2049)? cos takiego, nie rozumiem do ko�ca
															//wi�c musimy w tej sytuacji przesuwamy na dlugosc bufora opozniajacego minus pozycja 
	{
		delayBuffer.copyFromWithRamp(channel, bufferWritePosition, bufferData, bufferLength, 1, 1);
	}
	else
	{
		const int bufferRemaning = delayBufferLength - bufferWritePosition; //index 
		delayBuffer.copyFromWithRamp(channel, bufferWritePosition, bufferData, bufferRemaning, 1, 1);

		delayBuffer.copyFromWithRamp(channel, 0, bufferData, bufferLength - bufferRemaning, 1, 1);	//to co zosta�o wklejamy na pocz�tek. 
																										//czyli to co zosta�o odci�te (ca�y bufor to 
																										//bufferRemaining + ta reszta kt�r� tu doklejamy
	}
}


void ReverbEngine::copyBackToCurrentBuffer(AudioBuffer<float>& buffer, int channel, const float* bufferData, const float* delayBufferData,
	const int bufferLength, const int delayBufferLength, int delayTime)
{
	//int delayTime = 500;
	const int bufferReadPosition = static_cast<int>(delayBufferLength + bufferWritePosition - (sampleRate_ * delayTime / 1000)) % delayBufferLength;
	//ile pr�bek dalej b�dziemy 'wkleja�' sygna�. 
	//wi�c musi by� to int. Modulo zn�w �eby 'zawin��' sygna�
	//w tym buforze - indeks idzie na pocz�tek
	//ten static_cast<int> to po prostu rzutowanie na int, tak 
	//samo jakby napisa� (int)zmienna. �eby upewni� si�, �e 
	//liczymy modulo z inta (bo to index wi�c musi by� intem)
	//jest plus mWritePosition �eby�my si� przesuwali po prostu 
	//i czytali za ka�dym razem mWritePosition dalej (bo wczytujemy 
	//te� przesuwaj�c si� o taki krok)




	if (delayBufferLength > bufferLength + bufferReadPosition)			//ten if to analogicznie jak przy czytaniu - dop�ku jest wystarczaj�ca ilo�� pr�bek 
																		//w buforze op�niaj�cym �eby wzi�� stamt�d ilo�� pr�bek r�wn� d�ugo�ci naszego bufora,
																		//to dodajemy kawa�ek o d�ugo�ci naszego bufora (z przesz�o�ci) do naszego bufora
	{
		buffer.copyFromWithRamp(channel, 0, delayBufferData + bufferReadPosition, bufferLength, 1, 1);
	}
	else																//je�eli nie mamy tyle pr�bek (bufor op�niaj�cy si� 'ko�czy') to bierzemy kawa�ek z ko�ca
																		//i reszt� z pocz�tku 
	{
		const int bufferRemaining = delayBufferLength - bufferReadPosition;		//ile warto�ci zosta�o w delay bufferze
		buffer.copyFromWithRamp(channel, 0, delayBufferData + bufferReadPosition, bufferRemaining, 1, 1);
		buffer.copyFromWithRamp(channel, bufferRemaining, delayBufferData, bufferLength - bufferRemaining, 1, 1);
	}
	//buffer.applyGain(0.9);
}

void ReverbEngine::addDelayWithCurrentBuffer(int channel, const int bufferLength,
	const int delayBufferLength, float* bufferData, int delayTimesNumber)
{
	//const float* dryRead = dryBuffer.getReadPointer()
	//float amplitudeMultiplier = 0.68 / (delayTimesNumber);
	float amplitudeMultiplier = 0.65 / (delayTimesNumber);
	//int amplitudeMultiplier_ = Random::getSystemRandom().nextInt(Range<int>(1, 12));
	//float amplitudeMultiplier = (float)amplitudeMultiplier_ / 10;
	//float amplitudeMultiplier = 0.15;
	if (delayBufferLength > bufferLength + bufferWritePosition)
	{

		delayBuffer.addFromWithRamp(channel, bufferWritePosition, bufferData, bufferLength, amplitudeMultiplier, amplitudeMultiplier);		//ostatnie 2 argumenty - jak szybko zanika 
																														// - mno�enie amplitudy z ka�dym odbiciem

	}
	else
	{
		const int bufferRemaining = delayBufferLength - bufferWritePosition;
		delayBuffer.addFromWithRamp(channel, bufferWritePosition, bufferData, bufferRemaining, amplitudeMultiplier, amplitudeMultiplier);
		delayBuffer.addFromWithRamp(channel, 0, bufferData, bufferLength - bufferRemaining, amplitudeMultiplier, amplitudeMultiplier);

	}
	//mDelayBuffer.applyGain(0.7);
}