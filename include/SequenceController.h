#pragma once
#include "Turntable.h"

class SequenceController {
public:
    SequenceController(Turntable& table);

    void startSequence(int* segments, int count);
    void update();
    bool isRunning() const;

private:
    Turntable& _table;

    static const int MAX_SEGMENTS = 20;
    int _segments[MAX_SEGMENTS];
    int _count = 0;
    int _index = 0;

    enum Phase {
        HOME_BEFORE,
        RUNNING,
        HOME_AFTER,
        DONE
    };

    Phase _phase = DONE;

    bool _waiting = false;
    unsigned long _waitStart = 0;

    void startWait();
};