#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "Arduino.h"
// Abstract base class. All the actuators inherit from this.
class Actuator {
  private:
    int pin;      // The Arduino pin number the actuator is connected to
    bool active;  // tracks whether the actuator is on or off


  public:
    Actuator(int pin);

    // = 0 makes these pure virtual. Subclasses must implement their own version
    virtual void activate() = 0;
    virtual void deactivate() = 0;

    //returns the status of the actuator
    bool getStatus();

  protected:
    // getPin() gives subclasses read-only access to the private pin variable
    int getPin();

    // active is private so subclasses cannot access it directly.
    // setActive() is a protected method that allows subclasses
    // to update active in a controlled way, keeping the data hidden.
    void setActive(bool state);
};

#endif
