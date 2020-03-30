/*
  ==============================================================================

    LateReverb.cpp
    Created: 29 Mar 2020 11:22:16am
    Author:  Adam

  ==============================================================================
*/

#include "LateReverb.h" 


void LateReverb::addLateReverb(std::vector<int>& delayTimes)
{
	for (int line = 0; line < this->lateReverbNumLines - 1; ++line)
	{

		//delayTimes [line + (numberDelayLines - this->lateReverbNumLines) - 1] = this->highDelayTime + primeNumbers[Random::getSystemRandom().nextInt(Range<int>(0, primeNumbers.size()-5))] * line;
																		//liczba pierwsza, żeby nie było interferencji
		delayTimes [line + (numberDelayLines - this->lateReverbNumLines) - 1] = this->highDelayTime 
																				+ Random::getSystemRandom().nextInt(Range<int>(-50, 340));  //czyli np. 310, 320, 330
	}
}
