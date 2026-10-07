#pragma once

#include "Arduino.h"

class PinAuthenticator {
public:
    PinAuthenticator();

    bool validatePin(const String& inputPin);
    void resetAttempts();
    bool isLockedOut();
    void setValidPin(const String& newPin);

private:
    String m_validPin;
    int    m_maxAttempts;
    int    m_attempts;
    unsigned long m_lockoutStart;  // add this TO UML DIAGRAM
};