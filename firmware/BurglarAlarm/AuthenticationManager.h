#pragma once

#include "Arduino.h"
#include "PinAuthenticator.h"

class AuthenticationManager {
public:
    AuthenticationManager();

    void authenticationSelect(const String& mode);
    void displayMode() const;
    bool authenticate(const String& inputPin, bool faceDetected = false);
    bool isLockedOut();
    String getMode() const { return m_mode; }
    void setPin(const String& newPin);

private:
    String           m_mode;
    PinAuthenticator m_pinAuthenticator;
};