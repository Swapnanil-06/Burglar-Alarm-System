#include "PinAuthenticator.h"

PinAuthenticator::PinAuthenticator()
    : m_validPin("1234")
    , m_maxAttempts(3)
    , m_attempts(0)
    , m_lockoutStart(0)  // add this
{}

bool PinAuthenticator::validatePin(const String& inputPin) {
    if (isLockedOut()) return false;
    if (inputPin == m_validPin) {
        resetAttempts();
        return true;
    }
    ++m_attempts;
    if (m_attempts >= m_maxAttempts) {
        m_lockoutStart = millis();  // add this
        Serial.println("LOCKED OUT for 60 seconds");
    }
    return false;
}

void PinAuthenticator::resetAttempts() {
    m_attempts     = 0;
    m_lockoutStart = 0;  // add this
}

bool PinAuthenticator::isLockedOut() {
    if (m_attempts >= m_maxAttempts) {
        if (millis() - m_lockoutStart < 60000) {  // 60 seconds
            return true;
        } else {
            resetAttempts();  // auto reset after 60 seconds
            return false;
        }
    }
    return false;
}

void PinAuthenticator::setValidPin(const String& newPin) {
    m_validPin = newPin;
}