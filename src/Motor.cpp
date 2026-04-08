#include "Motor.h"

Motor::Motor(uint8_t pwm, uint8_t in1, uint8_t in2)
    : _pwm(pwm), _in1(in1), _in2(in2) {

    pinMode(_pwm, OUTPUT);
    pinMode(_in1, OUTPUT);
    pinMode(_in2, OUTPUT);
    stop();
}

void Motor::cw(uint8_t pwm) {
    digitalWrite(_in1, HIGH);
    digitalWrite(_in2, LOW);
    analogWrite(_pwm, pwm);
}

void Motor::ccw(uint8_t pwm) {
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, HIGH);
    analogWrite(_pwm, pwm);
}

void Motor::stop() {
    analogWrite(_pwm, 0);
}