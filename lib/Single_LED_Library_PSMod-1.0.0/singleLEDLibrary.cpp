#include "singleLEDLibrary.h"
#include "Arduino.h"

sllib::sllib(int pin)
{
    // make sure to use pwm ports here
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    Serial.begin(9600);
    _pin = pin;
}

void sllib::update()
{
    switch (runningFunction)
    {
    case 0:
        break;
    case 1:
        patternSingle(arrP);
        break;
    case 2:
        breathSingle(speedp);
        break;
    case 3:
        flickerSingle();
        break;
    case 4:
        blinkSingle(speedp);
        break;
    case 5:
        blinkRandomSingle(speedp, timep);
        break;
    case 6:
        patternPWMSingle(arrP, arrL, pwm0, pwm1);
        break;
    default:
        break;
    }
}

void sllib::setOnSingle()
{
    runningFunction = 0;
    digitalWrite(_pin, HIGH);
}

void sllib::setPWMSingle(const byte duty)
{
    runningFunction = 0;
    analogWrite(_pin, duty);
}

void sllib::setOffSingle()
{
    runningFunction = 0;
    digitalWrite(_pin, LOW);
}

void sllib::setRandomBlinkSingle(int minTime, int maxTime)
{
    runningFunction = 5;
    speedp = minTime;
    timep = maxTime;
}

void sllib::setPatternSingle(const int pattern[])
{
    if (arrP != 0)
    {
        delete[] arrP;
    }
    arrP = new int[LENGTH(pattern)];

    for (int i = 0; i < LENGTH(pattern); i++)
    {
        arrP[i] = pattern[i];
    }
    arrL = LENGTH(pattern);
    runningFunction = 1;
}

void sllib::setPatternPWMSingle(const int pattern[], const int l, const byte pwm0, const byte pwm1)
{
    this->pwm0 = pwm0;
    this->pwm1 = pwm1;

    if (runningFunction == 6)
        return;

    int t = 0;
    if (arrP != 0)
    {
        delete[] arrP;
    }
    arrP = new int[l];

    for (int i = 0; i < l; i++)
    {
        arrP[i] = pattern[i];
        t += arrP[i];
    }
    newMillis = millis();
    newMillis -= newMillis % t;
    arrL = l; // LENGTH(pattern);
    counter = 0;
    runningFunction = 6;
}

void sllib::setBreathSingle(int speed)
{
    runningFunction = 2;
    speedp = speed;
}

void sllib::setFlickerSingle()
{
    runningFunction = 3;
}

void sllib::setBlinkSingle(int speed)
{
    runningFunction = 4;
    speedp = speed;
}

// function for pattern based blinking, first entry always turns the led high
void sllib::patternSingle(const int pattern[])
{

    if (counter < LENGTH(pattern))
    {
        if ((newMillis + pattern[counter]) < millis())
        {
            // milOld = millis();
            newMillis += pattern[counter];
            if (counter % 2 == 0)
            {
                digitalWrite(_pin, LOW);
            }
            else
            {
                digitalWrite(_pin, HIGH);
            }
            counter++;
        }
    }
    else
    {
        counter = 0;
    }
}

void sllib::patternPWMSingle(const int pattern[], const int l, const byte pwm0, const byte pwm1)
{
    if (newMillis <= millis())
    {
        if (counter >= arrL)
            counter = 0;

        newMillis += pattern[counter];
        if (counter % 2 == 0 && pattern[counter])
        {
            analogWrite(_pin, pwm0);
            // digitalWrite(_pin,HIGH);
        }
        else
        {
            analogWrite(_pin, pwm1);
        }
        counter++;
    }
}

// simple breating function
void sllib::breathSingle(int speed)
{
    if ((newMillis + 1) < millis())
    {
        newMillis = millis();
        float it = 0;
        it = (exp(sin(millis() / (float)speed * PI)) - 0.36787944) * 108.0;
        analogWrite(_pin, it);
    }
}

// simple flickering function
void sllib::flickerSingle(int intMin, int intMax)
{
    if ((newMillis + 100) < millis())
    {
        newMillis = millis();
        float it = 0;
        it = random(intMin, intMax);
        analogWrite(_pin, it);
    }
}

// overload for instant use
void sllib::flickerSingle()
{
    if ((newMillis + 100) < millis())
    {
        newMillis = millis();
        float it = 0;
        it = random(80, 255);
        analogWrite(_pin, it);
    }
}

// overload for speed controll
void sllib::flickerSingle(int intMin, int intMax, int speed)
{
    if ((newMillis + speed) < millis())
    {
        newMillis = millis();
        float it = 0;
        it = random(intMin, intMax);
        analogWrite(_pin, it);
    }
}

// simple blinking function
void sllib::blinkSingle(int speed)
{
    if ((newMillis + speed) < millis())
    {
        newMillis = millis();
        if (ioBlink == false)
        {
            digitalWrite(_pin, HIGH);
            ioBlink = true;
        }
        else
        {
            digitalWrite(_pin, LOW);
            ioBlink = false;
        }
    }
}

// overload to allow for async blinking
void sllib::blinkSingle(int timeHigh, int timeLow)
{
    if (ioBlink == false)
    {
        if ((newMillis + timeHigh) < millis())
        {
            newMillis = millis();
            digitalWrite(_pin, LOW);
            ioBlink = true;
        }
    }
    else
    {
        if ((newMillis + timeLow) < millis())
        {
            newMillis = millis();
            digitalWrite(_pin, HIGH);
            ioBlink = false;
        }
    }
}

// blinking with randomised delay
void sllib::blinkRandomSingle(int minTime, int maxTime)
{
    if ((newMillis + rndTemp) < millis())
    {
        newMillis = millis();
        rndTemp = random(minTime, maxTime);
        if (ioBlink == false)
        {
            digitalWrite(_pin, HIGH);
            ioBlink = true;
        }
        else
        {
            digitalWrite(_pin, LOW);
            ioBlink = false;
        }
    }
}