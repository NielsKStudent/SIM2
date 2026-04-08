#pragma once
#include <Arduino.h>

class AngleSensor {
public:
    void begin();
    float getAngle();        // 0–360 logisch
    void calibrateForward(); // huidige positie = vooruit

private:
    float _offset = 0.0;
};
