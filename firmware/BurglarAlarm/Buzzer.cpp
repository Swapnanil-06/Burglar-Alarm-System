#include "Buzzer.h"

Buzzer::Buzzer(int pin, int frequency)
    : Actuator(pin), frequency(frequency) {
}

void Buzzer::activate() {
    tone(getPin(), frequency);  // plays tone at the given frequency in Hz
    setActive(true);
}

void Buzzer::deactivate() {
    noTone(getPin());  // stops the tone
    setActive(false);
}


void Buzzer::beep(int duration) {
    activate();
    delay(duration);   // duration in milliseconds
    deactivate();
}
