#include "buzzer.h"
#include "mbed.h"
  
using namespace mbed;
 
Beep::Beep(PinName pin) : _pwm(pin)
{
    _pwm.write(0.0);
}
 
void Beep::nobeep()
{
    _pwm.write(0.0);
}
 
/** Beep with given frequency and duration.
 *
 * @param frequency - the frequency of the tone in Hz
 * @param time - the duration of the tone in seconds
 */
void Beep::beep(float freq, float time)
{
 
    _pwm.period(1.0/freq);
    _pwm.write(0.5);
}