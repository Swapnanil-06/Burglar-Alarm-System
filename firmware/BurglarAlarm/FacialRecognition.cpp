#include "FacialRecognition.h"

FacialRecognition::FacialRecognition(unsigned long timeout)
    : m_faceDetected(false),
      m_lastDetectionTime(0),
      m_timeout(timeout)
{}

void FacialRecognition::begin() {
    // nothing needed yet
}

void FacialRecognition::update() {
    // Timeout safety only — serial reading moved to processMessage()
    // so the main loop owns the serial buffer exclusively
    if (millis() - m_lastDetectionTime > m_timeout) {
        m_faceDetected = false;
    }
}

void FacialRecognition::reset() {
    m_faceDetected = false;
}

void FacialRecognition::processMessage(const String& msg) {
    if (msg == "FACE_OK") {
        m_faceDetected = true;
        m_lastDetectionTime = millis();
        Serial.println("[Face] Recognised");

    } else if (msg == "FACE_FAIL") {
        m_faceDetected = false;
        Serial.println("[Face] Not recognised");
    }
}

bool FacialRecognition::isFaceDetected() const {
    return m_faceDetected;
}