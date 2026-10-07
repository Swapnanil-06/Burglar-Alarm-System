#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#include "Sensor.h"
#include "PIRSensor.h"
#include "MagneticSensor.h"
#include "SensorConfig.h"

#define MAX_SENSORS 8

// Manages all sensors and gives the .ino one interface
// for starting, stopping and polling them all.
// SR 3.1.3 — increase MAX_SENSORS for future expansion.
class SensorManager {
  private:
    Sensor*      sensors[MAX_SENSORS];
    int          sensorCount;
    SensorConfig config;

  public:
    SensorManager();

    void    addSensor(Sensor* sensor);
    void    removeSensor(String sensorId);
    Sensor* getSensor(String sensorId);

    void startAll();    // activate + calibrate every sensor (call on ARM)
    void stopAll();     // deactivate every sensor (call on disarm)
    void update();      // call every loop() — delegates to each sensor's update()

    bool anyTriggered() const;  // true if any active sensor is triggered

    // fills caller-supplied arrays with IDs and triggered states
    // used by AdminInterface::sensorstatus() (SR 6.3)
    void getStatus(String ids[], bool states[], int& count) const;
};

#endif