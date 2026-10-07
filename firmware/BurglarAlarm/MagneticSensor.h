#ifndef MAGNETICSENSOR_H
#define MAGNETICSENSOR_H

#include "Sensor.h"

// Reed switch sensor detects door-open events.
// NC (normally closed) wiring with INPUT_PULLUP:
//   door closed = magnet present = pin LOW  = safe
//   door open   = magnet removed = pin HIGH = triggered
// A cut wire also reads HIGH, which counts as tamper.
// Inherits triggered, rawValue, log[], logCount from Sensor.
class MagneticSensor : public Sensor {
  private:
    float  fieldStrength;  // last raw pin reading
    float  threshold;      // above this value the door is considered open
    String polarity;       // "NC" or "NO"
    int    pin;

  public:
    MagneticSensor(String id, String name, SensorConfig config, int pin,
                   float threshold, String polarity);

    // Overrides the pure virtual functions defined in Sensor
    void calibrate() override;  // sets INPUT_PULLUP and records baseline
    void logEvent()  override;  // writes timestamped string to log[]
    void update()    override;  // reads pin, updates triggered

    float  getFieldStrength() const;
    String getPolarity()      const;
    void   setThreshold(float value);

    // returns true if an NC sensor reads HIGH unexpectedly (wire cut / ripped off)
    bool isTampered() const;
};

#endif