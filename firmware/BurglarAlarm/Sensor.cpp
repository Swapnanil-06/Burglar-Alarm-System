#include "Sensor.h"

Sensor::Sensor(String id, String name, SensorConfig config)
    : sensorID(id), name(name), isActive(false), config(config),
      triggered(false), rawValue(0.0), logCount(0) {
}

void Sensor::activate() {
    isActive = true;
}

void Sensor::deactivate() {
    isActive  = false;
    triggered = false;  // clear trigger when sensor goes offline
}

bool Sensor::getIsActive() const {
    return isActive;
}

bool Sensor::isTriggered() const {
    return triggered;
}

String Sensor::getSensorId() const {
    return sensorID;
}

String Sensor::getSensorID() const {
    return sensorID;
}

String Sensor::getName() const {
    return name;
}