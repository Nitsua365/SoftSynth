//
//  Oscillator.hpp
//  AudioFramework
//
//  Created by Austin Blanchard on 5/31/24.
//

#ifndef Oscillator_hpp
#define Oscillator_hpp

#include <functional>

#include "Types.h"

namespace Env
{

struct Envelope
{
    double attackTime{0.5};
    double decayTime{1.0};
    double sustainAmp{0.15};
    double releaseTime{1.0};
    
    double keyPressedTimeOn;
    double keyPressedTimeOff;
};

}

namespace Osc
{

class Oscillator
{
private:
    static constexpr double FREQ_TO_ANGULAR_VEL{2.0 * M_PI};
    static constexpr double ANGULAR_VEL_TO_FREQ{2.0 / M_PI};
    static constexpr double MAX_AMP{0.2};
    
public:
    Oscillator(Types::Wave p_wave, double frequency = 440.0, double phase = 0.0);
    ~Oscillator();
    
    void setFrequency(double p_freq);
    double getFrequency() const;
    
    void incrementPhase(double p_phaseSample);
    void setPhase(double p_phase);
    double getPhase() const;
    
    double getAngularVelocity() const;
    
    double getSampleAtCurrentPhase();
    
    void setAmplitude(double p_amp);
    
//    void setEnvelope(Env::Envelope* p_env);
        
private:
    double frequency;
    double phase;
    double amplitude{MAX_AMP}; // set to this for speaker safety
            
    Types::Wave oscWave;
};

}

#endif /* Oscillator_hpp */
