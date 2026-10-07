#ifndef ACTUATORMANAGER_H
#define ACTUATORMANAGER_H

#include "Buzzer.h"
#include "LED.h"
#include "SolenoidLock.h"

// Manages all actuators and provides high-level alarm operations
class ActuatorManager {
  private:

    // Typed references allow access to subclass-specific methods
    Buzzer& buzzer;
    LED& statusLED;
    SolenoidLock& lock;

    // Array of base-class pointers — allows all actuators to be
    // controlled together using polymorphism
    static const int NUM_ACTUATORS = 3;
    Actuator* actuators[NUM_ACTUATORS];


  public:
    ActuatorManager(Buzzer& buzzer, LED& led, SolenoidLock& lock);
    void stopBuzzerAndLED();

    void soundAlarm();       // turns buzzer on
    void stopAlarm();        // turns buzzer off
    void lockDoor();         // locks the solenoid
    void unlockDoor();       // unlocks the solenoid
    void showArmedStatus();  // flashes LED to show that the system is armed

    void deactivateAll(); // deactivates all the actuators using the pointer array
};

#endif
