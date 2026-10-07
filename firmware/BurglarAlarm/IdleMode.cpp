#include "IdleMode.h"
#include "SensorManager.h"
#include "ActuatorManager.h"
#include "DisplayManager.h"
#include "LogManager.h"

// these global functions are defined in the .ino file
extern SensorManager&   getSensorManager();
extern ActuatorManager& getActuatorManager();
extern DisplayManager&  getDisplayManager();
extern LogManager&      getLogManager();
extern void             requestAuthentication();

IdleMode::IdleMode() {
}

// SR 1.2 — on entering Idle: sensors off, door unlocked, alarm silenced
void IdleMode::enter() {
    getSensorManager().stopAll();
    getActuatorManager().unlockDoor();
    getActuatorManager().stopAlarm();
    getActuatorManager().deactivateAll();
    getDisplayManager().setbuffer("IDLE");
    getLogManager().record("MODE_IDLE");
}

void IdleMode::exit() {
    // nothing to tear down
}

// input 1 = arm request
void IdleMode::handleInput(int input) {
    if (input == 1) {
        requestAuthentication();
    }
}

void IdleMode::update() {
    // Idle is passive — .ino reads keypad and calls handleInput()
}

String IdleMode::getModeName() const {
    return "IDLE";
}