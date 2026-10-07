#include "Actuator.h"

Actuator::Actuator(int pin) : pin(pin), active(false) {

    pinMode(pin, OUTPUT);
}

bool Actuator::getStatus() {
    return active;
}

int Actuator::getPin() {
    return pin;
}

void Actuator::setActive(bool state) {
    active = state;
}
