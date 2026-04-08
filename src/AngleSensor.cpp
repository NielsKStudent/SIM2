#include "AngleSensor.h"
#include <Wire.h>
#include <EEPROM.h>

#define AS5600_ADDR 0x36
#define RAW_ANGLE_MSB 0x0C
#define EEPROM_ADDR_OFFSET 0

void AngleSensor::begin() {
    Wire.begin();
    Wire.setClock(400000);
    EEPROM.get(EEPROM_ADDR_OFFSET, _offset);
    if (isnan(_offset) || _offset < 0 || _offset >= 360) {
        _offset = 0.0;
    }
}

float AngleSensor::getAngle() {
    Wire.beginTransmission(AS5600_ADDR);
    Wire.write(RAW_ANGLE_MSB);
    Wire.endTransmission(false);
    Wire.requestFrom(AS5600_ADDR, 2);

    int msb = Wire.read();
    int lsb = Wire.read();

    float raw = ((msb & 0x0F) << 8 | lsb) * 360.0 / 4096.0;
    float angle = raw - _offset;

    while (angle < 0) angle += 360;
    while (angle >= 360) angle -= 360;

    return angle;
}

void AngleSensor::calibrateForward() {
    Wire.beginTransmission(AS5600_ADDR);
    Wire.write(RAW_ANGLE_MSB);
    Wire.endTransmission(false);
    Wire.requestFrom(AS5600_ADDR, 2);

    int msb = Wire.read();
    int lsb = Wire.read();

    _offset = ((msb & 0x0F) << 8 | lsb) * 360.0 / 4096.0;
    EEPROM.put(EEPROM_ADDR_OFFSET, _offset);
}
