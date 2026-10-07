#ifndef ALARMMODE_H
#define ALARMMODE_H

#include "Mode.h"

// Buzzer and LED active. Only authentication can stop it.
// SR 1.5   — visual and audible alarms active.
// SR 1.5.3 — logs alarm start timestamp and duration.
// SR 4.2.3 — buzzer auto-stops after 20 minutes (UK regulation).
class AlarmMode : public Mode {
  private:
    long alarmStartTime;  // millis() when alarm began
    bool buzzerStopped;   // true once the 20-min auto-stop fires

  public:
    AlarmMode();

    // Overrides the pure virtual functions defined in Mode
    void   enter()                override;
    void   exit()                 override;
    void   handleInput(int input) override;
    void   update()               override;
    String getModeName()    const override;
};

#endif