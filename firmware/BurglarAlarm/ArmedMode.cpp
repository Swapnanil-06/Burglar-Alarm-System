#include "ArmedMode.h"
#include "SensorManager.h"
#include "ActuatorManager.h"
#include "DisplayManager.h"
#include "LogManager.h"

extern SensorManager&   getSensorManager();
extern ActuatorManager& getActuatorManager();
extern DisplayManager&  getDisplayManager();
extern LogManager&      getLogManager();
extern void             requestAuthentication();
extern void             sensorTriggerManager();
extern unsigned long EXIT_DELAY_MS;
ArmedMode::ArmedMode() : exitDelayStart(0), exitDelayOver(false) {
}

void ArmedMode::enter() {
    exitDelayStart = millis();
    exitDelayOver  = false;

    // SR 4.1.1 — solenoid locks when system transitions to Armed
    getActuatorManager().lockDoor();

    // SR 1.3.4 — flash LED to confirm arm within 1 second
    getActuatorManager().showArmedStatus();

    // SR 1.3.1 — all enabled sensors actively monitored
    getSensorManager().startAll();

    getDisplayManager().setbuffer("ARMED");
    getLogManager().record("MODE_ARMED");
}

void ArmedMode::exit() {
    // sensors keep running — CountdownMode still needs them
    // stopAll() is called by IdleMode::enter() after a successful disarm
}

// input 0 = disarm request
void ArmedMode::handleInput(int input) {
    if (input == 0) {
        requestAuthentication();
    }
}

void ArmedMode::update() {
    // poll all active sensors every loop
    getSensorManager().update();

    // SR 1.3.5 — ignore triggers during 30-second exit delay
    if (!exitDelayOver) {
        if (millis() - exitDelayStart >= EXIT_DELAY_MS) {
            exitDelayOver = true;
            getDisplayManager().setbuffer("ARMED-LIVE");
            Serial.println("Exit delay over — sensors now live");
        }
        return;
    }

    // any triggered sensor starts the countdown
    if (getSensorManager().anyTriggered()) {
        Serial.println("Sensor triggered");
        sensorTriggerManager();
    }
}

String ArmedMode::getModeName() const {
    return "ARMED";
}