//
//  AudioDriver.cpp
//  AudioFramework
//
//  Created by Austin Blanchard on 6/2/24.
//

#include "AudioDriver.hpp"

namespace AudioDriver
{
    
OSStatus AudioCB::callback(void *inRefCon, AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames, AudioBufferList *ioData)
{
    if (!output) return 1;
    
    double waveSample;
            
    for (auto& osc : output->getGenerators())
    {
        waveSample = osc->getAngularVelocity() / output->getSampleRate();
        
        int16_t* buffer = static_cast<int16_t*>(ioData->mBuffers[0].mData);
                
        for (uint32_t frame = 0; frame < inNumberFrames; frame++)
        {
            buffer[frame * 2] = (int16_t)(osc->getSampleAtCurrentPhase() * INT16_MAX);         // Left channel
            buffer[frame * 2 + 1] = (int16_t)(osc->getSampleAtCurrentPhase() * INT16_MAX);     // Right channel
                                    
            // increment the phase by the sample of the wave
            osc->incrementPhase(waveSample);
        }
        
    }
    
    return noErr;
}

void AudioCB::registerOutput(AudioOutput* p_Output)
{
    if (!output) output = p_Output;
}

}
