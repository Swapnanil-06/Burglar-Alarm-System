#ifndef IDLEMODE_H
#define IDLEMODE_H

#include "Mode.h"

// System is disarmed — no sensors active, door unlocked, no alarms.
// SR 1.2 — system shall be in Idle when disarmed.
class IdleMode : public Mode {
  public:
    IdleMode();

    // Overrides the pure virtual functions defined in Mode
    void   enter()                override;
    void   exit()                 override;
    void   handleInput(int input) override;
    void   update()               override;
    String getModeName()    const override;
};

#endif