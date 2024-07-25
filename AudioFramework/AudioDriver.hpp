//
//  AudioDriver.hpp
//  AudioFramework
//
//  Created by Austin Blanchard on 6/1/24.
//

#ifndef AudioDriver_hpp
#define AudioDriver_hpp

#include "AudioOutput.hpp"

namespace AudioDriver
{

static AudioOutput* output = nullptr;

class AudioCB
{
public:
    AudioCB() = default;
            
    static OSStatus callback(void *inRefCon, AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames, AudioBufferList *ioData);

    void registerOutput(AudioOutput* p_Output);
};

}

#endif /* AudioDriver_hpp */
