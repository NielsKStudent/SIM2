#pragma once
#include "Motor.h"
#include "AngleSensor.h"

class Turntable {
public:
    Turntable(Motor& motor, AngleSensor& sensor);
    void setTargetSegment(int segment);
    bool update(); // true = target bereikt

private:
    Motor& _motor;
    AngleSensor& _sensor;

    float _targetAngle = 0.0;

    static constexpr int NUM_SEGMENTS = 24;
    static constexpr float SEGMENT_ANGLE = 360.0 / NUM_SEGMENTS;
    static constexpr float ANGLE_TOLERANCE = 1.0;
    static constexpr uint8_t PWM_VALUE = 25;
};