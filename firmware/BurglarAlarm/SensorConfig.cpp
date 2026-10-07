#include "SensorConfig.h"

SensorConfig::SensorConfig()
    : samplingRate(500), timeout(5000), retryCount(3) {
}

void SensorConfig::setSamplingRate(int rate) {
    samplingRate = rate;
}

void SensorConfig::setTimeout(int ms) {
    timeout = ms;
}

int SensorConfig::getSamplingRate() const {
    return samplingRate;
}

int SensorConfig::getTimeout() const {
    return timeout;
}

bool SensorConfig::validate() const {
    return (samplingRate > 0 && timeout > 0 && retryCount >= 0);
}