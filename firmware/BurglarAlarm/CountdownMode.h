#pragma once

#include "CountdownTimer.h"

class CountdownMode {
public:
    CountdownMode();

    void displayCountdown()             const;
    void countdownAudio()               const;
    void setCountdownMode(bool enable);
    void update(float deltaTime);

private:
    bool           m_enable;
    CountdownTimer m_timer;
};