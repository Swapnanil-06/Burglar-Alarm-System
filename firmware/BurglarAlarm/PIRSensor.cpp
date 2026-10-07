#include "PIRSensor.h"

PIRSensor::PIRSensor(String id, String name, SensorConfig config, int pin,
                     float sensitivity, float detectionRange)
    : Sensor(id, name, config),
      sensitivity(sensitivity),
      detectionRange(detectionRange),
      motionDetected(false),
      pin(pin) {
}

void PIRSensor::calibrate() {
    pinMode(pin, INPUT);
    delay(2000);  // PIR sensors need ~2s to stabilise after power-on
}

void PIRSensor::logEvent() {
    if (logCount < 20) {
        log[logCount] = "PIR triggered at " + String(millis()) + "ms";
        logCount++;
    }
}

void PIRSensor::update() {
    if (!getIsActive()) return;

    rawValue  = digitalRead(pin);
    triggered = (rawValue == HIGH);  // HC-SR505 is digital-only; HIGH == motion detected

    // only log on the rising edge — first frame of a new motion event
    if (triggered && !motionDetected) {
        motionDetected = true;
        logEvent();
    }

    if (!triggered) {
        motionDetected = false;
    }
}

float PIRSensor::getSensitivity() const {
    return sensitivity;
}

void PIRSensor::setSensitivity(float value) {
    sensitivity = value;
}

bool PIRSensor::isMotionDetected() const {
    return motionDetected;
}
