#pragma once

class CountdownTimer {
public:
    CountdownTimer();

    void displayCountdown() const;
    void startCountdown();
    void stopCountdown();
    void resumeCountdown();
    void setCountdown(float time);
    void clearCountdown();
    void update(float deltaTime);
    bool isFinished()       const;

private:
    float m_initialTime;
    float m_remainingTime;
    bool  m_enabled;
};