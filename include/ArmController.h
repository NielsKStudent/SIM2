#pragma once
#include <Arduino.h>
#include <Servo.h>

class ArmController {
public:
    ArmController();

    void begin();

    // acties per vakje
    void actionFullSequence();
    void actionSecondPick();
    void actionFinalRelease();

private:
    // actuatoren
    Servo gripper;
    Servo diabolo;

    // arm DC motor pins
    int armIn1;
    int armIn2;
    int armEn;

    // hulpfuncties
    void armStop();
    void armDown(int ms);
    void armUp(int ms);

    void diaboloForward(int ms);
    void diaboloBackward(int ms);
};