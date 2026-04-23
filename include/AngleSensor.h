#pragma once
#include <Arduino.h>

class AngleSensor {
public:
    void begin();
    float getAngle();   // geeft -180 .. +180
    void zero();        // huidige positie = 0°

private:
    float _zeroOffset = 0.0;
};