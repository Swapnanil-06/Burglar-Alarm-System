#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include "SensorConfig.h"

// Abstract base class. All sensors inherit from this.
// triggered and rawValue are stored here so SensorManager
// can check them without needing a separate data struct.
class Sensor {
  private:
    String       sensorID;
    String       name;
    bool         isActive;
    SensorConfig config;

  protected:
    bool  triggered;  // true when sensor has detected an event
    float rawValue;   // last hardware reading

    // in-memory log — each entry is a timestamped string
    String log[20];
    int    logCount;

    // gives subclasses read-only access to private identity fields
    String getSensorID() const;
    String getName()     const;

  public:
    Sensor(String id, String name, SensorConfig config);

    // = 0 makes these pure virtual. Subclasses must implement their own version
    virtual void calibrate() = 0;
    virtual void logEvent()  = 0;
    virtual void update()    = 0;

    void   activate();
    void   deactivate();
    bool   getIsActive()   const;
    bool   isTriggered()   const;
    String getSensorId()   const;
};

#endif