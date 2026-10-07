#ifndef FACIAL_RECOGNITION_H
#define FACIAL_RECOGNITION_H

#include <Arduino.h>

class FacialRecognition {
private:
    bool m_faceDetected;
    unsigned long m_lastDetectionTime;
    unsigned long m_timeout;

public:
    FacialRecognition(unsigned long timeout = 2000);

    void begin();
    void update();
    void reset();                       // handles timeout only — no serial reading
    void processMessage(const String& msg); // call from main loop after reading serial
    bool isFaceDetected() const;
};

#endif