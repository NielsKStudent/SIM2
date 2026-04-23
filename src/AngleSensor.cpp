#include "AngleSensor.h"
#include <Wire.h>

#define AS5600_ADDR 0x36
#define RAW_ANGLE_MSB 0x0C

static float readRawAngle() {
    Wire.beginTransmission(AS5600_ADDR);
    Wire.write(RAW_ANGLE_MSB);
    Wire.endTransmission(false);
    Wire.requestFrom(AS5600_ADDR, 2);

    int msb = Wire.read();
    int lsb = Wire.read();

    int raw = ((msb & 0x0F) << 8) | lsb;
    return raw * 360.0f / 4096.0f;
}

void AngleSensor::begin() {
    Wire.begin();
    Wire.setClock(400000);
    _zeroOffset = readRawAngle();
}

void AngleSensor::zero() {
    _zeroOffset = readRawAngle();
}

float AngleSensor::getAngle() {
    float angle = readRawAngle() - _zeroOffset;
    if (angle > 180) angle -= 360;
    if (angle < -180) angle += 360;
    return angle;
}
