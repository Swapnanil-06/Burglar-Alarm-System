#ifndef MODE_H
#define MODE_H

#include <Arduino.h>

// Abstract base class for all system modes.
// In the Arduino build there is no Controller object — the .ino file
// holds global instances and global functions instead.
// Modes call those global functions directly (e.g. setMode(), getSensorManager()).
// The pure virtuals below must be implemented by every subclass.
class Mode {
  public:
    Mode() {}

    // = 0 makes these pure virtual. Subclasses must implement their own version
    virtual void   enter()                = 0;  // called once when entering this mode
    virtual void   exit()                 = 0;  // called once when leaving this mode
    virtual void   handleInput(int input) = 0;  // called when keypad input arrives
    virtual void   update()               = 0;  // called every loop()
    virtual String getModeName()    const = 0;  // returns mode name for LCD display
};

#endif