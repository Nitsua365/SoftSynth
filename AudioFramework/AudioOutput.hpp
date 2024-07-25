//
//  AudioOutput.hpp
//  AudioFramework
//
//  Created by Austin Blanchard on 5/29/24.
//

#ifndef AudioOutput_hpp
#define AudioOutput_hpp

#include <iostream>
#include <vector>
#include <functional>

#include "Oscillator.hpp"

#include <AudioUnit/AudioOutputUnit.h>
#include <CoreAudio/CoreAudio.h>
#include <AudioToolbox/AudioComponent.h>
#include <AudioToolbox/AudioToolbox.h>

namespace AudioDriver
{

class AudioCB;

class AudioOutput
{
public:
    AudioOutput(AudioDriver::AudioCB* cb, double p_sampleRate = 44100.0, uint32_t p_numChannels = 2, uint32_t p_bitProcessing = 16, uint32_t p_frameSize = 4, uint32_t p_packetSize = 4);
    ~AudioOutput();
        
    void play();
    
    void keyOn();
    void keyOff();
    
    void addOscillator(Osc::Oscillator* p_osc);
    
    void addMainEnvelope(Env::Envelope* p_env);
    
    double getSampleRate() const;
    
    const std::vector<Osc::Oscillator*>& getGenerators();
    
private:
    double sampleRate;
    uint32_t numChannels;
    uint32_t bitRate;
    uint32_t frameSize;
    uint32_t packetSize;
    
    AudioDriver::AudioCB* AudioCBDriver{nullptr};
    Env::Envelope* mainEnv{nullptr};
    
    std::vector<Osc::Oscillator*> generators;
    // std::vector<Filter::Filter*> filters;
        
    AURenderCallbackStruct callbackStruct;
    AudioUnit outputUnit;
    
};

}

#endif /* AudioOutput_hpp */
