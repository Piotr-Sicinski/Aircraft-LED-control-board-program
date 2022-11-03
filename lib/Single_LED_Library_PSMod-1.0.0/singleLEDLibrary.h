/*singleLEDLibrary
 * A library for non interupting lighting effects for single LED's
 * Pim Ostendorf - 2017.11.24
 */

#ifndef singleLEDLibrary_h
#define singleLEDLibrary_h
#include "Arduino.h"
#include <limits.h>

#define LENGTH(array) 4 //(sizeof array / sizeof *array)

class sllib
{
public:
    // public variables and fucntions
    sllib(int pin);

    // breathing
    void breathSingle(int speed);

    // blink
    void blinkSingle(int speed);
    void blinkSingle(int timeHigh, int timeLow);
    void blinkRandomSingle(int minTime, int maxTime);

    // flicker
    void flickerSingle();
    void flickerSingle(int intMin, int intMax);
    void flickerSingle(int intMin, int intMax, int speed);

    // blink pattern
    void patternSingle(const int pattern[]);

    // future update function
    void update();
    void setPatternSingle(const int pattern[]);
    void setBreathSingle(int speed);
    void setFlickerSingle();
    void setBlinkSingle(int speed);
    void setRandomBlinkSingle(int minTime, int maxTime);
    void setOffSingle();

    // Piotr update
    void patternPWMSingle(const int pattern[], const int l, const byte pwm0, const byte pwm1);

    void setOnSingle();
    void setPWMSingle(const byte duty);
    void setPatternPWMSingle(const int pattern[], const int l, const byte pwm0, const byte pwm1);

private:
    // private variables and fucntion
    // global variables
    int _pin;
    unsigned long newMillis;
    int runningFunction = 0;
    int speedp;
    int timep;

    int *arrP = 0;
    int arrL = 0;

    // variables for blinking
    bool ioBlink = false;
    int rndTemp = 0;

    // variables for pattern
    int counter = 0;
    byte pwm0 = UCHAR_MAX / 4,
         pwm1 = UCHAR_MAX * 3 / 4;
};

#endif