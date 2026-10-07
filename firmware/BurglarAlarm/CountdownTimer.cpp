#include "CountdownTimer.h"
#include <Arduino.h>  // REQUIRED for Serial

CountdownTimer::CountdownTimer()
    : m_initialTime(0.0f)
    , m_remainingTime(0.0f)
    , m_enabled(false)
{}

void CountdownTimer::setCountdown(float time) {
    m_initialTime   = time;
    m_remainingTime = m_initialTime;
}

void CountdownTimer::startCountdown() {
    m_remainingTime = m_initialTime;
    m_enabled       = true;
}

void CountdownTimer::stopCountdown() {
    m_enabled = false;
}

void CountdownTimer::resumeCountdown() {
    if (m_remainingTime > 0.0f) {
        m_enabled = true;
    }
}

void CountdownTimer::clearCountdown() {
    m_enabled       = false;
    m_initialTime   = 0.0f;
    m_remainingTime = 0.0f;
}

void CountdownTimer::update(float deltaTime) {
    if (!m_enabled) return;

    m_remainingTime -= deltaTime;

    if (m_remainingTime <= 0.0f) {
        m_remainingTime = 0.0f;
        m_enabled       = false;
    }
}

bool CountdownTimer::isFinished() const {
    return !m_enabled && m_remainingTime == 0.0f && m_initialTime > 0.0f;
}

void CountdownTimer::displayCountdown() const {
    Serial.print("Time remaining: ");
    Serial.print(m_remainingTime);
    Serial.println("s");
}