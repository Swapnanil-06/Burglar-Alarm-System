#include "LED.h"

LED::LED(int pin, String colour)
    : Actuator(pin), colour(colour), flashing(false) {
}

void LED::activate() {
    digitalWrite(getPin(), HIGH); // LED ON
    setActive(true);
    flashing = false;
}

void LED::deactivate() {
    digitalWrite(getPin(), LOW);  // LED OFF
    setActive(false);
    flashing = false;
}

void LED::flash(int interval) {
    // Switches the LED on/off on each call from loop()
    flashing = true;
    if (getStatus()) {
        digitalWrite(getPin(), LOW);
        setActive(false);
    } else {
        digitalWrite(getPin(), HIGH);
        setActive(true);
    }
    delay(interval);  // interval in milliseconds controls flash speed
}
