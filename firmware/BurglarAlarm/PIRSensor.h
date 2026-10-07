#ifndef PIRSENSOR_H
#define PIRSENSOR_H

#include "Sensor.h"

// PIR sensor detects motion via infrared changes.
// Inherits triggered, rawValue, log[], logCount from Sensor.
class PIRSensor : public Sensor {
  private:
    float sensitivity;     // rawValue above this means motion detected
    float detectionRange;  // rated range in metres
    bool  motionDetected;  // tracks state so we only log on rising edge
    int   pin;

  public:
    PIRSensor(String id, String name, SensorConfig config, int pin,
              float sensitivity, float detectionRange);

    // Overrides the pure virtual functions defined in Sensor
    void calibrate() override;  // sets pin mode and waits for warm-up
    void logEvent()  override;  // writes timestamped string to log[]
    void update()    override;  // reads pin, updates triggered

    float getSensitivity()       const;
    void  setSensitivity(float value);
    bool  isMotionDetected()     const;
};

#endif