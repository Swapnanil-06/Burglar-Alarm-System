#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <Arduino.h>

// Manages short audio cues for mode changes and state feedback.
// This is separate from the Buzzer alarm — the Buzzer is for sustained
// alarm sounds, AudioManager is for brief feedback beeps.
// SR 1.6.3 — mode changes indicated by a short audio cue.
// SR 5.2.1 — audio prompts for state changes (accessibility).
class AudioManager {
  private:
    bool triggered;      // true while a cue is playing
    int  triggerTime;    // millis() when the cue started

  public:
    AudioManager();

    // plays a short beep — call on every mode change
    void trigger();

    // stops any playing cue
    void silence();

    // returns when the last cue was triggered
    int gettriggertime();

    // call from loop() — auto-silences after the cue duration
    void update();
};

#endif