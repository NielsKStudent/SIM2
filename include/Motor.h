#pragma once
#include <Arduino.h>

class Motor {
public:
    Motor(uint8_t pwm, uint8_t in1, uint8_t in2);
    void cw(uint8_t pwm);
    void ccw(uint8_t pwm);
    void stop();

private:
    uint8_t _pwm, _in1, _in2;
};