#ifndef MBED_BEEP_H
#define MBED_BEEP_H

#include "mbed.h"
namespace mbed {
 /* Class: Beep
 *  A class which uses pwm to controle a beeper to generate sounds.
 */
class Beep {
  
public:
 
   Beep (PinName pin);//Specified PwmOut pin for buzzer
 
/** Beep with given frequency and duration.
 *
* @param frequency - the frequency of the tone in Hz
* @param time - the duration of the tone in seconds
 */
    void beep (float frequency, float time);
 
    void nobeep();
  
private :
    PwmOut _pwm;
    Timeout toff;
};
 
}
#endif