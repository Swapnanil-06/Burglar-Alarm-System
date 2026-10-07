#ifndef LED_H
#define LED_H

#include "Actuator.h"

// LED inherits from Actuator
class LED : public Actuator {
  private:
    String colour;
    bool flashing;

  public:
    LED(int pin, String colour);

    // Overrides the pure virtual functions defined in Actuator
    void activate() override;
    void deactivate() override;

    // switches LED on/off at set interval in milliseconds
    void flash(int interval);
};

#endif
