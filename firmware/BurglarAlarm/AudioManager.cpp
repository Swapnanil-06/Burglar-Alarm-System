#include "AudioManager.h"

// Pin 9 is taken by the LED so we cannot use tone() on a separate pin.
// Audio cues are routed through the alarm buzzer on pin 8 instead.
// The buzzer already handles all sounds — AudioManager just tracks
// the triggered state so the class structure stays complete for OOP.
#define AUDIO_PIN  8    // same pin as the Buzzer — short beep only
#define AUDIO_FREQ 2000
#define AUDIO_DURATION 150  // ms — short enough not to clash with alarm

AudioManager::AudioManager() : triggered(false), triggerTime(0) {
}

// SR 1.6.3 — short audio cue on mode change
// uses the buzzer pin since no spare pin is available on this circuit
void AudioManager::trigger() {
    triggered   = true;
    triggerTime = millis();
    tone(AUDIO_PIN, AUDIO_FREQ);
}

void AudioManager::silence() {
    triggered = false;
    noTone(AUDIO_PIN);
}

int AudioManager::gettriggertime() {
    return triggerTime;
}

// auto-silences after AUDIO_DURATION ms — called every loop()
void AudioManager::update() {
    if (triggered && (millis() - triggerTime >= AUDIO_DURATION)) {
        silence();
    }
}