#include "SensorManager.h"

SensorManager::SensorManager() : sensorCount(0) {
    for (int i = 0; i < MAX_SENSORS; i++) {
        sensors[i] = 0;
    }
}

void SensorManager::addSensor(Sensor* sensor) {
    if (sensorCount < MAX_SENSORS) {
        sensors[sensorCount] = sensor;
        sensorCount++;
    }
}

void SensorManager::removeSensor(String sensorId) {
    for (int i = 0; i < sensorCount; i++) {
        if (sensors[i] != 0 && sensors[i]->getSensorId() == sensorId) {
            for (int j = i; j < sensorCount - 1; j++) {
                sensors[j] = sensors[j + 1];
            }
            sensorCount--;
            sensors[sensorCount] = 0;
            return;
        }
    }
}

Sensor* SensorManager::getSensor(String sensorId) {
    for (int i = 0; i < sensorCount; i++) {
        if (sensors[i] != 0 && sensors[i]->getSensorId() == sensorId) {
            return sensors[i];
        }
    }
    return 0;
}

// SR 1.3.1 — activate and calibrate all sensors when arming
void SensorManager::startAll() {
    for (int i = 0; i < sensorCount; i++) {
        if (sensors[i] != 0) {
            sensors[i]->activate();
            sensors[i]->calibrate();
        }
    }
}

void SensorManager::stopAll() {
    for (int i = 0; i < sensorCount; i++) {
        if (sensors[i] != 0) {
            sensors[i]->deactivate();
        }
    }
}

void SensorManager::update() {
    for (int i = 0; i < sensorCount; i++) {
        if (sensors[i] != 0) {
            sensors[i]->update();
        }
    }
}

bool SensorManager::anyTriggered() const {
    for (int i = 0; i < sensorCount; i++) {
        if (sensors[i] != 0 && sensors[i]->getIsActive() && sensors[i]->isTriggered()) {
            return true;
        }
    }
    return false;
}

// SR 6.3 — used by AdminInterface to show sensor status on LCD / Serial
void SensorManager::getStatus(String ids[], bool states[], int& count) const {
    count = 0;
    for (int i = 0; i < sensorCount; i++) {
        if (sensors[i] != 0) {
            ids[count]    = sensors[i]->getSensorId();
            states[count] = sensors[i]->isTriggered();
            count++;
        }
    }
}