#include "SequenceController.h"
#include <Arduino.h>

#define WAIT_TIME_MS 5000
#define HOME_SEGMENT 1

SequenceController::SequenceController(Turntable& table)
: _table(table) {}

bool SequenceController::isRunning() const {
    return _phase != DONE;
}

void SequenceController::startSequence(int* segments, int count) {
    if (count <= 0) return;

    _count = min(count, MAX_SEGMENTS);
    for (int i = 0; i < _count; i++) {
        _segments[i] = segments[i];
    }

    _index = 0;
    _waiting = false;
    _phase = HOME_BEFORE;

    Serial.println("Start sequentie: eerst naar HOME (segment 1)");
    _table.setTargetSegment(HOME_SEGMENT);
}

void SequenceController::startWait() {
    _waiting = true;
    _waitStart = millis();
}

void SequenceController::update() {
    if (_phase == DONE) return;

    if (_waiting) {
        if (millis() - _waitStart >= WAIT_TIME_MS) {
            _waiting = false;
        }
        return;
    }

    if (!_table.update()) return; // nog niet op doel

    switch (_phase) {

    case HOME_BEFORE:
        Serial.println("Bij segment 1 (home)");
        startWait();
        _phase = RUNNING;
        _table.setTargetSegment(_segments[_index]);
        break;

    case RUNNING:
        Serial.print("Bij segment ");
        Serial.println(_segments[_index]);
        startWait();

        _index++;
        if (_index < _count) {
            _table.setTargetSegment(_segments[_index]);
        } else {
            Serial.println("Sequentie klaar, terug naar HOME");
            _phase = HOME_AFTER;
            _table.setTargetSegment(HOME_SEGMENT);
        }
        break;

    case HOME_AFTER:
        Serial.println("Klaar, terug op segment 1");
        startWait();
        _phase = DONE;
        break;

    case DONE:
        break;
    }
}