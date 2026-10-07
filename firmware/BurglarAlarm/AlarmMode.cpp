#include "AlarmMode.h"
#include "ActuatorManager.h"
#include "DisplayManager.h"
#include "LogManager.h"

extern ActuatorManager& getActuatorManager();
extern DisplayManager&  getDisplayManager();
extern LogManager&      getLogManager();
extern void             requestAuthentication();

AlarmMode::AlarmMode() : alarmStartTime(0), buzzerStopped(false) {
}

// SR 1.5 — sound alarm; SR 1.5.3 — log start timestamp
void AlarmMode::enter() {
    alarmStartTime = millis();
    buzzerStopped  = false;

    // SR 1.5.1 — buzzer activates continuously
    getActuatorManager().soundAlarm();

    getDisplayManager().setbuffer("!! ALARM !!");
    getLogManager().record("MODE_ALARM_START");
}

// SR 1.5.3 — log how long the alarm lasted
// SR 1.5.5 — all indicators stop within 2s of authentication
void AlarmMode::exit() {
    long duration = millis() - alarmStartTime;

    getActuatorManager().stopAlarm();
    getActuatorManager().stopBuzzerAndLED();

    getLogManager().record("MODE_ALARM_END | duration:" + String(duration) + "ms");
    Serial.println("Alarm ended. Duration: " + String(duration) + "ms");
}

// any keypad press while in alarm triggers an auth request
void AlarmMode::handleInput(int input) {
    requestAuthentication();
}

void AlarmMode::update() {
    // SR 4.2.3 — UK regulation: auto-stop buzzer at 20 minutes
    if (!buzzerStopped && (millis() - alarmStartTime >= 1200000)) {
        getActuatorManager().stopAlarm();
        buzzerStopped = true;
        Serial.println("Buzzer auto-stopped at 20 min limit");
        getLogManager().record("BUZZER_AUTOSTOP_20MIN");
    }

    // SR 1.5.2 — LED keeps flashing as visual indicator
    // getActuatorManager().flashLED(500);
}

String AlarmMode::getModeName() const {
    return "ALARM";
}