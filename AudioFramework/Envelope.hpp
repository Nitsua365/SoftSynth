//
//  Envelope.hpp
//  AudioFramework
//
//  Created by Austin Blanchard on 6/27/24.
//

#ifndef Envelope_hpp
#define Envelope_hpp

#include <stdio.h>

namespace Env
{

class Envelope
{
public:
    Envelope();
    ~Envelope() = default;
    
    double attackTime;
    double decayTime;
    double sustainVol;
    double releaseTime;
    
//    virtual void callback() = 0;
};

}

#endif /* Envelope_hpp */
