#ifndef SENSORCONFIG_H
#define SENSORCONFIG_H

// Holds shared configuration values for all sensors.
// One instance lives in SensorManager and is passed into each Sensor.
class SensorConfig {
  private:
    int samplingRate;  // how often sensors are polled in ms
    int timeout;       // ms before a reading is considered stale
    int retryCount;    // number of read retries on failure

  public:
    SensorConfig();

    void setSamplingRate(int rate);
    void setTimeout(int ms);
    int  getSamplingRate() const;
    int  getTimeout()      const;
    bool validate()        const;
};

#endif