#include "ArmController.h"

// ===== constructor =====
ArmController::ArmController() {
    // vaste pins (PAS AAN ALS NODIG)
    armIn1 = 4;
    armIn2 = 5;
    armEn  = 6;
}

// ===== init =====
void ArmController::begin() {
    pinMode(armIn1, OUTPUT);
    pinMode(armIn2, OUTPUT);
    pinMode(armEn, OUTPUT);

    gripper.attach(2);   // gripper servo
    diabolo.attach(10);  // continuous servo

    armStop();
    gripper.write(90);   // open
    diabolo.write(90);   // stop
}

// ===== DC ARM MOTOR =====
void ArmController::armStop() {
    digitalWrite(armIn1, LOW);
    digitalWrite(armIn2, LOW);
    analogWrite(armEn, 0);
}

void ArmController::armDown(int ms) {
    digitalWrite(armIn1, HIGH);
    digitalWrite(armIn2, LOW);
    analogWrite(armEn, 255);
    delay(ms);
    armStop();
}

void ArmController::armUp(int ms) {
    digitalWrite(armIn1, LOW);
    digitalWrite(armIn2, HIGH);
    analogWrite(armEn, 255);
    delay(ms);
    armStop();
}

// ===== CONTINUOUS SERVO =====
void ArmController::diaboloForward(int ms) {
    diabolo.write(180);
    delay(ms);
    diabolo.write(90);
}

void ArmController::diaboloBackward(int ms) {
    diabolo.write(0);
    delay(ms);
    diabolo.write(90);
}

// ===== ACTIES =====

// vakje 1: OUDE LANGE SEQUENCE (PAS HIER AAN)
void ArmController::actionFullSequence() {
    Serial.println("ARM: FULL SEQUENCE");

    gripper.write(135);
    delay(800);

    armDown(900);
    diaboloForward(1500);

    gripper.write(90);
    delay(300);

    armUp(1500);
    diaboloForward(800);
}

// vakje 2: kort oppakken
void ArmController::actionSecondPick() {
    Serial.println("ARM: SECOND PICK");

    armDown(600);

    gripper.write(90);
    delay(300);

    gripper.write(135);
    delay(300);

    armUp(800);
}

// vakje 3: loslaten
void ArmController::actionFinalRelease() {
    Serial.println("ARM: FINAL RELEASE");

    armDown(600);
    gripper.write(90);
    delay(500);
}