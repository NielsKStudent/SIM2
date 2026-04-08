#include "Turntable.h"

static float angleError(float target, float current) {
    float error = target - current;
    while (error > 180) error -= 360;
    while (error < -180) error += 360;
    return error;
}

Turntable::Turntable(Motor& motor, AngleSensor& sensor)
: _motor(motor), _sensor(sensor) {}

void Turntable::setTargetSegment(int segment) {
    int logicalIndex = (segment + NUM_SEGMENTS - 1) % NUM_SEGMENTS;
    _targetAngle = logicalIndex * SEGMENT_ANGLE;
}

bool Turntable::update() {
    float current = _sensor.getAngle();
    float error = angleError(_targetAngle, current);

    if (abs(error) <= ANGLE_TOLERANCE) {
        _motor.stop();
        return true;
    } else if (error > 0) {
        _motor.cw(PWM_VALUE);
    } else {
        _motor.ccw(PWM_VALUE);
    }
    return false;
}