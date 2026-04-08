#include <Arduino.h>
#include "Motor.h"
#include "AngleSensor.h"
#include "Turntable.h"
#include "SequenceController.h"

Motor motor(9, 7, 8);
AngleSensor sensor;
Turntable table(motor, sensor);
SequenceController sequence(table);

// snelle regel-loop
constexpr unsigned long CONTROL_INTERVAL_US = 5000; // 200 Hz
unsigned long lastControl = 0;

// langzame logica
constexpr unsigned long SEQUENCE_INTERVAL_MS = 50;
unsigned long lastSequence = 0;

void setup() {
    Serial.begin(115200);
    sensor.begin();

    Serial.println("Klaar.");
    Serial.println("Geef sequentie met: seq 5,18,9");
}

void loop() {
    // -------- SERIAL INPUT --------
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        cmd.toLowerCase();

        Serial.print("> ");
        Serial.println(cmd);

        if (cmd.startsWith("seq ")) {
            int segments[20];
            int count = 0;

            String list = cmd.substring(4);
            while (list.length() > 0 && count < 20) {
                int comma = list.indexOf(',');
                if (comma == -1) {
                    segments[count++] = list.toInt();
                    break;
                } else {
                    segments[count++] = list.substring(0, comma).toInt();
                    list = list.substring(comma + 1);
                }
            }

            sequence.startSequence(segments, count);
        }
    }

    unsigned long nowMicros = micros();
    unsigned long nowMillis = millis();

    // ✅ snelle regeling
    if (nowMicros - lastControl >= CONTROL_INTERVAL_US) {
        lastControl = nowMicros;
        table.update();
    }

    // ✅ sequentielogica
    if (nowMillis - lastSequence >= SEQUENCE_INTERVAL_MS) {
        lastSequence = nowMillis;
        sequence.update();
    }
}