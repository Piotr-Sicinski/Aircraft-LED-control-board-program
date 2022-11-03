#include "main.h"
// 2 channels from rc and current microcontroler does not support more
rc_c rc0(rc0Pin), rc1(rc1Pin);

// global variable for dimming all leds at once
uint ONduty = DUTY_MAX;
// variable for potentiometer read
uint potValue = POT_MAX;

sllib lbLed(lbPin);
sllib sbLed(sbPin);
sllib szLed(szPin);
sllib obLed(obPin);
sllib ozLed(ozPin);

sllib biLed(LED_BUILTIN);

void setup()
{
    attachInterrupt(digitalPinToInterrupt(rc0Pin), rc0intFunk, CHANGE);
    attachInterrupt(digitalPinToInterrupt(rc1Pin), rc1intFunk, CHANGE);

    setNonchangingLeds();
    // always changes brightness and updates 100% mode led
    ONduty = 2 * DUTY_MAX;
}

void loop()
{
    // updating duty for global dimming
    potValue = analogRead(potPin);
    if (abs(ONduty - potValue / 4) > DUTY_MAX / 16)
    {
        // adding something to prevent complete turn off
        ONduty = potValue / 4 + DUTY_MAX / 16;
        if (ONduty >= DUTY_MAX)
        {
            ONduty = DUTY_MAX;
            biLed.setOnSingle();
        }
        else
        {
            biLed.setOffSingle();
        }

        setNonchangingLeds();
    }

    // place to add functions for each channel form rc
    switch (rc0.sigDecode())
    {
    case 0:
        break;
    case 1:
        lbLed.setOffSingle();
        szLed.setPWMSingle(ONduty);
        break;
    case 2:
        lbLed.setPWMSingle(ONduty);
        szLed.setPWMSingle(ONduty);
        break;
    case 3:
        lbLed.setPWMSingle(ONduty);
        szLed.setPatternPWMSingle(szPattern, LENGTH(szPattern), ONduty, 0);
        break;
    default:
        break;
    }

    switch (rc1.sigDecode())
    {
    case 0:
        break;
    case 1:
        obLed.setPWMSingle(ONduty);
        ozLed.setOffSingle();
        break;
    case 2:
        obLed.setOffSingle();
        ozLed.setPWMSingle(ONduty);
        break;
    case 3:
        obLed.setOffSingle();
        ozLed.setPatternPWMSingle(ozPattern, LENGTH(ozPattern), ONduty, 0);
        break;
    default:
        break;
    }

    // needs to be called frequently to make patterns regular
    ledUpdate();
}

void ledUpdate()
{
    lbLed.update();
    sbLed.update();
    szLed.update();
    obLed.update();
    ozLed.update();
}

void rc0intFunk()
{
    rc0.intFunk();
}

void rc1intFunk()
{
    rc1.intFunk();
}

void setNonchangingLeds()
{
    sbLed.setPatternPWMSingle(sbPattern, LENGTH(sbPattern), ONduty, ONduty / 8);
}
