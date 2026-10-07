#include "SolenoidLock.h"

SolenoidLock::SolenoidLock(int pin)
    : Actuator(pin), locked(true) {
}

void SolenoidLock::activate() {
    digitalWrite(getPin(), HIGH);  // locks the solenoid
    setActive(true);
}

void SolenoidLock::deactivate() {
    digitalWrite(getPin(), LOW);  // cuts power to unlock
    setActive(false);
}

void SolenoidLock::setLocked(bool state) {
    locked = state;
    if (locked) {
        activate();    // lock
    } else {
        deactivate();  // unlock
    }
}
