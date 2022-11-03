#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <singleLEDLibrary.h>

#define DUTY_MAX (UCHAR_MAX)
#define POT_MAX (1023)

typedef unsigned long ulong;
typedef unsigned int uint;

// Notacja: sbPin
// S/O/L - skrzydła, ogon, lądowania
// B/C/Z - kolor
#define rc0Pin 3
#define rc1Pin 2
#define potPin A3
#define lbPin 9
#define sbPin 11
#define szPin 5
#define obPin 6
#define ozPin 10

// paterns for patrn blinking
const int ozPattern[] = {0, 600, 300, 600},
          szPattern[] = {0, 600, 300, 600},
          sbPattern[] = {50, 100, 50, 1300};

const int pwmThreshold0 = 1200,
          pwmThreshold1 = 1800; // threshold levels for decoding 3 states of PWM

class rc_c
{
private:
    byte pin;
    byte state = 0;
    volatile uint64_t time = 0;
    volatile bool newTimeAv = false;

public:
    rc_c(byte pin) : pin(pin) {}

    // function called by interrupt and measurs pulse duration
    void intFunk();
    // returns decoded position (0-2) of rod on radio
    byte sigDecode();
};

void ledUpdate();
void setNonchangingLeds();

void rc0intFunk();
void rc1intFunk();

void rc_c::intFunk()
{
    if (digitalRead(pin) == HIGH)
    {
        // start measuring
        time = micros();
    }
    else
    {
        // stop measuring
        time = micros() - time;
        newTimeAv = true;
    }
}

byte rc_c::sigDecode()
{
    if (!newTimeAv)
        return 0;

    newTimeAv = false;
    if (time <= pwmThreshold0)
        state = 1;
    if (time > pwmThreshold0 && time <= pwmThreshold1)
        state = 2;
    if (time > pwmThreshold1)
        state = 3;
    return state;
}

#endif // MAIN_H