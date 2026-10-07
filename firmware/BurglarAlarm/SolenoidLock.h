#ifndef SOLENOIDLOCK_H
#define SOLENOIDLOCK_H

#include "Actuator.h"

// SolenoidLock inherits from Actuator and functions as a door locking mechanism
class SolenoidLock : public Actuator {
  private:
    bool locked;  // tracks the lock state separately from active

  public:
    SolenoidLock(int pin);

    // Overrides the pure virtual functions defined in Actuator
    void activate() override;
    void deactivate() override;

    // sets locked state and calls activate/deactivate accordingly
    void setLocked(bool state);
};

#endif
