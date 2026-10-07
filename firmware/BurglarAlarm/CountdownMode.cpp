#include "CountdownMode.h"
#include <Arduino.h>  // REQUIRED for Serial

CountdownMode::CountdownMode()
    : m_enable(false)
{}

void CountdownMode::setCountdownMode(bool enable) {
    m_enable = enable;
    if (!m_enable) {
        m_timer.clearCountdown();
    }
}

void CountdownMode::update(float deltaTime) {
    if (!m_enable) return;

    m_timer.update(deltaTime);

    if (m_timer.isFinished()) {
        countdownAudio();
    }
}

void CountdownMode::displayCountdown() const {
    if (!m_enable) return;
    m_timer.displayCountdown();
}

void CountdownMode::countdownAudio() const {
    if (!m_enable) return;

    // Placeholder for actual buzzer/speaker logic
    Serial.println("Countdown audio");
}