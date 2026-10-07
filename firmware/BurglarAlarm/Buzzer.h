#ifndef BUZZER_H
#define BUZZER_H

#include "Actuator.h"

// Buzzer inherits from Actuator
class Buzzer : public Actuator {
  private:
    int frequency;  // tone frequency in Hz

  public:
    Buzzer(int pin, int frequency);

    // Overrides the pure virtual functions defined in Actuator
    void activate() override;    // plays tone at set frequency
    void deactivate() override;  // stops the tone

    void beep(int duration);     // activates buzzer for a set duration in milliseconds
};

#endif