#include "AuthenticationManager.h"

AuthenticationManager::AuthenticationManager()
    : m_mode("both")
{}

void AuthenticationManager::authenticationSelect(const String& mode) {
    if (mode == "pin" || mode == "face" || mode == "both") {
        m_mode = mode;
    }
}

void AuthenticationManager::displayMode() const {
    Serial.print("Mode: ");
    Serial.println(m_mode);
}

bool AuthenticationManager::authenticate(const String& inputPin, bool faceDetected) {
    Serial.print("Authenticating with mode: ");
    Serial.println(m_mode);

    if (m_mode == "pin") {
        return m_pinAuthenticator.validatePin(inputPin);
    }
    else if (m_mode == "face") {
        if (faceDetected) {
            Serial.println("Face recognised!");
            return true;
        }
        Serial.println("Face NOT recognised!");
        return false;
    }
    else if (m_mode == "both") {
        bool pinValid  = m_pinAuthenticator.validatePin(inputPin);
        if (faceDetected) {
            Serial.println("Face recognised!");
        } else {
            Serial.println("Face NOT recognised!");
        }
        if (!pinValid || !faceDetected) {
            Serial.println("Both factors required - FAILED");
        }
        return pinValid && faceDetected;
    }

    return false;
}

bool AuthenticationManager::isLockedOut() {
    return m_pinAuthenticator.isLockedOut();
}

void AuthenticationManager::setPin(const String& newPin) {
    m_pinAuthenticator.setValidPin(newPin);
}