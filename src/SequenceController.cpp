#include "SequenceController.h"

#define WAIT_TIME 1000

SequenceController::SequenceController(Turntable& table)
: _table(table) {}

void SequenceController::startSequence(int* segments, int count) {
    _count = min(count, 3);
    for (int i = 0; i < _count; i++) {
        _segments[i] = segments[i];
    }

    _index = 0;
    _waiting = false;

    // ===== ARM INIT (eenmalig hier) =====
    pinMode(ARM_IN1, OUTPUT);
    pinMode(ARM_IN2, OUTPUT);
    pinMode(ARM_EN, OUTPUT);

    gripper.attach(2);
    diabolo.attach(3);

    gripper.write(90);
    diabolo.write(90);
    armStop();

    _table.setTargetSegment(_segments[0]);
}

void SequenceController::update() {
    if (_index >= _count) return;

    bool arrived = _table.update();

    if (_waiting) {
        if (millis() - _waitStart >= WAIT_TIME) {
            _waiting = false;
            _index++;

            if (_index < _count) {
                _table.setTargetSegment(_segments[_index]);
            } else {
                // terug naar 0
                _table.setTargetSegment(1);
            }
        }
        return;
    }

    if (arrived) {

        // ===== ARM ACTIES =====
        if (_index == 0) actionFullSequence();
        else if (_index == 1) actionSecondPick();
        else if (_index == 2) actionFinalRelease();

        _waiting = true;
        _waitStart = millis();
    }
}

// ===== ARM LOW LEVEL =====

void SequenceController::armStop() {
    digitalWrite(ARM_IN1, LOW);
    digitalWrite(ARM_IN2, LOW);
    analogWrite(ARM_EN, 0);
}

void SequenceController::armDown(int ms) {
    digitalWrite(ARM_IN1, HIGH);
    digitalWrite(ARM_IN2, LOW);
    analogWrite(ARM_EN, 255);
    delay(ms);
    armStop();
}

void SequenceController::armUp(int ms) {
    digitalWrite(ARM_IN1, LOW);
    digitalWrite(ARM_IN2, HIGH);
    analogWrite(ARM_EN, 255);
    delay(ms);
    armStop();
}

void SequenceController::diaboloForward(int ms) {
    diabolo.write(180);
    delay(ms);
    diabolo.write(90);
}

void SequenceController::diaboloBackward(int ms) {
    diabolo.write(0);
    delay(ms);
    diabolo.write(90);
}

// ===== ARM ACTIES =====

void SequenceController::actionFullSequence() {
    gripper.write(90);
    delay(300);

    diaboloBackward(1500);
    delay(300);

    armDown(900);
    delay(300);

    gripper.write(135);
    delay(300);

    diaboloForward(1300);
    delay(300);

    armUp(1500);
    delay(300);

    gripper.write(90);
    delay(300);

    diaboloForward(1000);
    delay(300);

    gripper.write(90);
    delay(300);

    armDown(400);

    gripper.write(135);
    delay(300);

    armUp(1100);
    delay(300);
    

    diaboloBackward(1100);
}

void SequenceController::actionSecondPick() {
    diaboloForward(1500);
    delay(300);

    armDown(400);

    gripper.write(90);
    delay(300);

    armUp(1100);
    delay(500);

    armDown(400);
    delay(500);

    gripper.write(135);
    delay(300);

    armUp(1100);
    delay(300);

    diaboloBackward(1100);
}

void SequenceController::actionFinalRelease() {

    diaboloForward(1500);
    delay(300);

    armDown(500);
    delay(300);

    gripper.write(90);
    delay(300);


    armUp(1000);
    delay(300);

    diaboloBackward(1100);
}