#pragma once
#include <Arduino.h>
#include <Servo.h>
#include "Turntable.h"

class SequenceController {
public:
    SequenceController(Turntable& table);

    void startSequence(int* segments, int count);
    void update();

private:
    // ===== REFERENCES =====
    Turntable& _table;

    // ===== SEQUENCE =====
    int _segments[3];
    int _count = 0;
    int _index = 0;
    bool _waiting = false;
    unsigned long _waitStart = 0;

    // ===== ARM HARDWARE =====
    Servo gripper;
    Servo diabolo;

    const int ARM_IN1 = 7;
    const int ARM_IN2 = 8;
    const int ARM_EN  = 9;

    // ===== ARM FUNCTIES =====
    void armStop();
    void armDown(int ms);
    void armUp(int ms);
    void diaboloForward(int ms);
    void diaboloBackward(int ms);

    void actionFullSequence();
    void actionSecondPick();
    void actionFinalRelease();
};
