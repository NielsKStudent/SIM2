#include "Turntable.h"
#include <Arduino.h>

static constexpr int PWM_MIN = 40;   // minimaal om wrijving te overwinnen
static constexpr int PWM_MAX = 60;   // max snelheid
static constexpr float KP = 1.2;     // krachtfactor
static constexpr float TOLERANCE = 1.0;


static float angleError(float target, float current) {
    float e = target - current;
    if (e > 180) e -= 360;
    if (e < -180) e += 360;
    return e;
}

Turntable::Turntable(Motor& motor, AngleSensor& sensor)
: _motor(motor), _sensor(sensor) {}

void Turntable::setTargetSegment(int segment) {
    // segment 1 = 0°, segment 13 = 180°
    int idx = (segment - 1 + NUM_SEGMENTS) % NUM_SEGMENTS;
    float angle = idx * SEGMENT_ANGLE;

    // ✅ harde limiet ±180
    if (angle > 180) angle -= 360;
    if (angle > 180) angle = 180;
    if (angle < -180) angle = -180;

    _targetAngle = angle;
}

bool Turntable::update() {
    float current = _sensor.getAngle();        // -180 .. +180
    float error   = _targetAngle - current;    // lineair, geen wrap

    // ✅ harde eindstops
    if ((current >= 180 && error > 0) ||
        (current <= -180 && error < 0)) {
        _motor.stop();
        return true;
    }

    // ✅ aangekomen
    if (abs(error) <= TOLERANCE) {
        _motor.stop();
        return true;
    }

    // ✅ proportionele snelheid
    int speed = abs(error) * KP;

    // begrenzen
    if (speed < PWM_MIN) speed = PWM_MIN;
    if (speed > PWM_MAX) speed = PWM_MAX;

    // ✅ richting
    if (error > 0) {
        _motor.cw(speed);
    } else {
        _motor.ccw(speed);
    }

    return false;
}