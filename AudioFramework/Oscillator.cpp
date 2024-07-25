//
//  Oscillator.cpp
//  AudioFramework
//
//  Created by Austin Blanchard on 5/31/24.
//

#include "Oscillator.hpp"

namespace Osc 
{

Oscillator::Oscillator(Types::Wave p_wave, double freq, double ph)
    : oscWave(p_wave), frequency(freq), phase(ph)
{
}

void Oscillator::setFrequency(double p_freq)
{
    this->frequency = p_freq;
}

void Oscillator::setPhase(double p_phase)
{
    this->phase = p_phase;
}

void Oscillator::setAmplitude(double p_amp)
{
    this->amplitude = (p_amp > MAX_AMP) ? MAX_AMP : p_amp;
}

void Oscillator::incrementPhase(double p_phaseSample)
{
    this->phase += p_phaseSample;
    
    if (this->phase >= FREQ_TO_ANGULAR_VEL) this->setPhase(this->phase - FREQ_TO_ANGULAR_VEL);
}

double Oscillator::getPhase() const
{
    return this->phase;
}

double Oscillator::getFrequency() const
{
    return this->frequency;
}

double Oscillator::getAngularVelocity() const
{
    return FREQ_TO_ANGULAR_VEL * this->frequency;
}

double Oscillator::getSampleAtCurrentPhase()
{
    switch (this->oscWave)
    {
        case Types::Wave::SINE:
            return this->amplitude * sin(this->phase);
        case Types::Wave::CLEAN_SAW:
            return this->amplitude * (2.0 * (this->phase / (2.0 * M_PI))) - 1.0;
        case Types::Wave::SQUARE:
            return this->amplitude * ((sin(this->phase) > 0.0) ? 1.0 : -1.0);
        case Types::Wave::TRIANGLE:
        default:
            return 0.0;
    }
}

}
