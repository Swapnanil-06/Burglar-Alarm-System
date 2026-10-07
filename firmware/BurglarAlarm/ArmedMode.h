#ifndef ARMEDMODE_H
#define ARMEDMODE_H

#include "Mode.h"

// Sensors actively monitor for intrusion, solenoid locked.
// SR 1.3   — sensors detect intrusion when system is armed.
// SR 1.3.5 — 30-second exit delay before sensors start triggering.
class ArmedMode : public Mode {
  private:
    long exitDelayStart;  // millis() when armed mode was entered
    bool exitDelayOver;   // flips true once the 30s window closes

  public:
    ArmedMode();

    // Overrides the pure virtual functions defined in Mode
    void   enter()                override;
    void   exit()                 override;
    void   handleInput(int input) override;
    void   update()               override;
    String getModeName()    const override;
};

#endif