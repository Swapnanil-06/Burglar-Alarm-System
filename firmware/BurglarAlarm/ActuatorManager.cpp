#include "ActuatorManager.h"

ActuatorManager::ActuatorManager(Buzzer& buzzer, LED& led, SolenoidLock& lock)
    : buzzer(buzzer), statusLED(led), lock(lock) {
    actuators[0] = &buzzer;
    actuators[1] = &led;
    actuators[2] = &lock;
}

void ActuatorManager::soundAlarm() {
    buzzer.activate();
    statusLED.activate();
}

void ActuatorManager::stopAlarm() {
    buzzer.deactivate();
    statusLED.deactivate();
}

// Stops buzzer + LED only — solenoid state is intentionally preserved
void ActuatorManager::stopBuzzerAndLED() {
    buzzer.deactivate();
    statusLED.deactivate();
}

void ActuatorManager::lockDoor() {
    lock.setLocked(true);
}

void ActuatorManager::unlockDoor() {
    lock.setLocked(false);
}

void ActuatorManager::showArmedStatus() {
    for (int i = 0; i < 3; i++) {
        statusLED.activate();
        delay(200);
        statusLED.deactivate();
        delay(200);
    }
}

// ⚠️ Only call this on full system shutdown or factory reset
// NEVER call during alarm/cooldown transitions — it unlocks the door
void ActuatorManager::deactivateAll() {
    for (int i = 0; i < NUM_ACTUATORS; i++) {
        actuators[i]->deactivate();
    }
}