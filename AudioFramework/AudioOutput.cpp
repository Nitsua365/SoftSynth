//
//  AudioOutput.cpp
//  AudioFramework
//
//  Created by Austin Blanchard on 5/29/24.
//

#include "AudioOutput.hpp"
#include "AudioDriver.hpp"

namespace AudioDriver 
{

AudioOutput::AudioOutput(AudioDriver::AudioCB* cb, double p_sampleRate, uint32_t p_numChannels, uint32_t p_bitProcessing, uint32_t p_frameSize, uint32_t p_packetSize) : AudioCBDriver(cb), sampleRate(p_sampleRate), numChannels(p_numChannels), bitRate(p_bitProcessing), frameSize(p_frameSize), packetSize(p_packetSize)
{
    OSStatus status;
    
    // Create an AudioComponentDescription
    AudioComponentDescription desc;
    desc.componentType = kAudioUnitType_Output;
    desc.componentSubType = kAudioUnitSubType_DefaultOutput;
    desc.componentManufacturer = kAudioUnitManufacturer_Apple;
    desc.componentFlags = 0;
    desc.componentFlagsMask = 0;
    
    // Find the default output audio unit
    AudioComponent defaultOutput = AudioComponentFindNext(NULL, &desc);
    if (defaultOutput == NULL) {
        std::cerr << "Can't find default output unit" << std::endl;
        exit(1);
    }
    
    
    // Create an AudioUnit instance
    if ((status = AudioComponentInstanceNew(defaultOutput, &outputUnit)) != noErr)
    {
        std::cerr << "Code: " << status << ", Cannot create Audio Component" << std::endl;
    }
    
    AudioStreamBasicDescription streamFormat;
    streamFormat.mSampleRate = sampleRate;
    streamFormat.mFormatID = kAudioFormatLinearPCM;
    streamFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
    streamFormat.mFramesPerPacket = 1;
    streamFormat.mChannelsPerFrame = numChannels; // stereo
    streamFormat.mBitsPerChannel = bitRate;
    streamFormat.mBytesPerPacket = packetSize;
    streamFormat.mBytesPerFrame = frameSize;
    
    if ((status = AudioUnitSetProperty(outputUnit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input, 0, &streamFormat, sizeof(streamFormat)) != noErr))
    {
        std::cerr << "Code: " << status << ", Cannot set audio unit property" << std::endl;
    }

    callbackStruct.inputProc = AudioCBDriver->callback;
    callbackStruct.inputProcRefCon = AudioCBDriver;
    if ((status = AudioUnitSetProperty(outputUnit,
                                    kAudioUnitProperty_SetRenderCallback,
                                    kAudioUnitScope_Input,
                                    0,
                                    &callbackStruct,
                                    sizeof(callbackStruct))) != noErr)
    {
        std::cerr << "Code: " << status << ", Cannot set audio output unit" << std::endl;
    }

    // Initialize the audio unit
    if ((status = AudioUnitInitialize(outputUnit)) != noErr)
    {
        std::cerr << "Code: " << status << ", Cannot Initialize audio unit" << std::endl;
    }
    
}

void AudioOutput::play()
{
    OSStatus status;
    
    // Start playback
    if ((status = AudioOutputUnitStart(outputUnit)) != noErr)
    {
        std::cerr << "Code: " << status << ", Cannot start audio unit" << std::endl;
    }

    // Keep the application running to allow audio to play
    std::cout << "Playing audio..." << std::endl;
    
    CFRunLoopRun();
}

double AudioOutput::getSampleRate() const
{
    return this->sampleRate;
}

const std::vector<Osc::Oscillator*>& AudioOutput::getGenerators()
{
    return this->generators;
}

void AudioOutput::addOscillator(Osc::Oscillator* p_osc)
{
    this->generators.push_back(p_osc);
}

void AudioOutput::addMainEnvelope(Env::Envelope* p_env)
{
    this->mainEnv = p_env;
}



}
