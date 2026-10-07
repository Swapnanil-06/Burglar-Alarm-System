#include "MagneticSensor.h"

MagneticSensor::MagneticSensor(String id, String name, SensorConfig config, int pin,
                                float threshold, String polarity)
    : Sensor(id, name, config),
      fieldStrength(0.0),
      threshold(threshold),
      polarity(polarity),
      pin(pin) {
}

void MagneticSensor::calibrate() {
    // The NC reed switch connects the input to GND while the door is closed.
    // The internal pull-up makes an open door or cut wire read HIGH.
    pinMode(pin, INPUT_PULLUP);
    fieldStrength = digitalRead(pin);  // record baseline at startup
}

void MagneticSensor::logEvent() {
    if (logCount < 20) {
        log[logCount] = "Magnetic triggered at " + String(millis()) + "ms";
        logCount++;
    }
}

void MagneticSensor::update() {
    if (!getIsActive()) return;

    float previous = fieldStrength;
    fieldStrength  = digitalRead(pin);
    rawValue       = fieldStrength;

    if (polarity == "NC") {
        // HIGH means the circuit is broken — door open or wire cut
        triggered = (fieldStrength > threshold);
    } else {
        // NO: LOW means door is open
        triggered = (fieldStrength < threshold);
    }

    // only log on rising edge
    if (triggered && previous <= threshold) {
        logEvent();
    }
}

float MagneticSensor::getFieldStrength() const {
    return fieldStrength;
}

String MagneticSensor::getPolarity() const {
    return polarity;
}

void MagneticSensor::setThreshold(float value) {
    threshold = value;
}

// an NC sensor reading HIGH when armed = wire cut or sensor removed
bool MagneticSensor::isTampered() const {
    if (polarity == "NC") {
        return (fieldStrength == HIGH);
    }
    return false;
}
