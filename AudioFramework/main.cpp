//
//  main.cpp
//  AudioFramework
//
//  Created by Austin Blanchard on 5/27/24.
//

#include <iostream>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <poll.h>
#include <termios.h>

#include "Types.h"

#include "AudioOutput.hpp"
#include "AudioDriver.hpp"
#include "Oscillator.hpp"

void setNonCanonicalMode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Function to restore terminal to its original state
void restoreTerminalMode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO); // Enable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void userInputLoop(Osc::Oscillator* p_Osc, AudioDriver::AudioOutput* p_Output)
{
    setNonCanonicalMode();
    
    constexpr double A4 = 440.0;

    const double C4 = pow(2.0, -9.0/12.0) * A4;
    const double CSh4 = pow(2.0, -8.0/12.0) * A4;
    const double D4 = pow(2.0, -7.0/12.0) * A4;
    const double DSh4 = pow(2.0, -6.0/12.0) * A4;
    const double E4 = pow(2.0, -5.0/12.0) * A4;
    const double F4 = pow(2.0, -4.0/12.0) * A4;
    const double FSh4 = pow(2.0, -3.0/12.0) * A4;
    const double G4 = pow(2.0, -2.0/12.0) * A4;
    const double GSh4 = pow(2.0, -1.0/12.0) * A4;
    const double ASh4 = pow(2.0, 1.0/12.0) * A4;
    const double B4 = pow(2.0, 2.0/12.0) * A4;
    const double C5 = C4 * 2.0;

    pollfd fds;
    fds.fd = STDIN_FILENO;
    fds.events = POLLIN;
    
    bool running = true;
    const int32_t ESCAPE = 27;
    
    while (running)
    {
        int ret = poll(&fds, 1, 10); // 10 ms timeout
        
        if (ret > 0) {
            
            if (fds.revents & POLLIN) {
                
                char c;
                read(STDIN_FILENO, &c, 1);
                
                switch (c)
                {
                    case 'a':
                        p_Osc->setFrequency(C4);
                        break;
                    case 'w':
                        p_Osc->setFrequency(CSh4);
                        break;
                    case 's':
                        p_Osc->setFrequency(D4);
                        break;
                    case 'e':
                        p_Osc->setFrequency(DSh4);
                        break;
                    case 'd':
                        p_Osc->setFrequency(E4);
                        break;
                    case 'f':
                        p_Osc->setFrequency(F4);
                        break;
                    case 't':
                        p_Osc->setFrequency(FSh4);
                        break;
                    case 'g':
                        p_Osc->setFrequency(G4);
                        break;
                    case 'y':
                        p_Osc->setFrequency(GSh4);
                        break;
                    case 'h':
                        p_Osc->setFrequency(A4);
                        break;
                    case 'u':
                        p_Osc->setFrequency(ASh4);
                        break;
                    case 'j':
                        p_Osc->setFrequency(B4);
                        break;
                    case 'k':
                        p_Osc->setFrequency(C5);
                        break;
                    case ESCAPE:
                        running = false;
                }
//                p_Output->keyOn();
            }
            else
            {
                std::cout << "OFF" << std::endl;
//                p_Output->keyOff();
                p_Osc->setAmplitude(0.0);
            }
            
        }
            
    }
    
    restoreTerminalMode();
    
}

int main(int argc, const char * argv[]) 
{
    // create a Sine wave oscillator
    Osc::Oscillator* osc = new Osc::Oscillator(Types::Wave::SINE);
    // create a Saw wave oscillator
//    Osc::Oscillator* osc = new Osc::Oscillator(Types::Wave::CLEAN_SAW);
    // create a Square wave oscillator
//    Osc::Oscillator* osc = new Osc::Oscillator(Types::Wave::SQUARE);
    
    Env::Envelope* env = new Env::Envelope();
        
    AudioDriver::AudioCB* mainCB = new AudioDriver::AudioCB();
    AudioDriver::AudioOutput* mainOutput = new AudioDriver::AudioOutput(mainCB);
    
    mainOutput->addMainEnvelope(env);
    
    mainCB->registerOutput(mainOutput);
    
    // add oscillator to main output
    mainOutput->addOscillator(osc);
    
    std::cout << "Press ESC to quit" << std::endl;
    
    // start a thread to listen for quitting
    std::thread userInputThread{&userInputLoop, osc, mainOutput};
    
    // start a thread to play audio
    std::thread audioThread{[mainOutput](){ mainOutput->play(); }};
            
    // block until the escape character is pressed
    userInputThread.join();
    
    // join the audio thread to the main thread
    audioThread.join();
        
    return 0;
}
