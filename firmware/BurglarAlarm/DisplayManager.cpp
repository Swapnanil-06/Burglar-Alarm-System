#include "DisplayManager.h"

DisplayManager::DisplayManager()
    : bufferSize(16), buffer(""), lastActivityTime(0), displayOn(false) {
}

// SR 1.6.1 — sets text and prints to Serial as a stand-in for the real LCD
// swap Serial.println for your lcd.print() call when LCD is wired up
void DisplayManager::setbuffer(String inputString) {
    buffer           = inputString;
    lastActivityTime = millis();
    displayOn        = true;

    Serial.println("[LCD] " + buffer);
    // lcd.clear();
    // lcd.print(buffer);
}

String DisplayManager::getbuffer() {
    return buffer;
}

int DisplayManager::getbuffersize() {
    return bufferSize;
}

void DisplayManager::clearbuffer() {
    buffer    = "";
    displayOn = false;

    // lcd.clear();
    // lcd.noBacklight();
}

// SR 1.6.2 — auto-off after 5 seconds of inactivity
// call this from loop() or from the .ino update block
void DisplayManager::update() {
    if (displayOn && (millis() - lastActivityTime >= 5000)) {
        clearbuffer();
        Serial.println("[LCD] off");
    }
}