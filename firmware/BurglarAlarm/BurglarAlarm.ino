// main.ino — Burglar Alarm System
// Structured as an AlarmController class to match the UML class diagram.
// setup() and loop() are thin Arduino entry points that delegate to the controller.

#include "ActuatorManager.h"
#include "SensorManager.h"
#include "AuthenticationManager.h"
#include "FacialRecognition.h"
#include "CountdownMode.h"
#include "IdleMode.h"
#include "ArmedMode.h"
#include "AlarmMode.h"
#include "LogManager.h"
#include "DisplayManager.h"

// Pin assignments for all hardware components
const int PIR_PIN      = 5;
const int MAG_PIN      = 6;
const int BUZZER_PIN   = 8;
const int LED_PIN      = 9;
const int SOLENOID_PIN = 10;

// NC reed switch with INPUT_PULLUP: HIGH means door is open.
// To verify polarity: send STATUS and move the magnet.
// If MAG:0 when open  → change to LOW
// If MAG:1 when open  → HIGH is correct, leave it
#define MAG_OPEN_STATE HIGH

// Internal states of the alarm system state machine
enum SysState {
    SYS_IDLE,
    SYS_EXIT_DELAY,
    SYS_ARMED,
    SYS_ENTRY_DELAY,
    SYS_ALARM,
    SYS_COOLDOWN
};

// EXIT_DELAY_MS is kept as a true global so ArmedMode.cpp can read it directly
// without any changes to that file. The controller updates this variable via
// the SET_EXIT: command, keeping both in sync.
unsigned long EXIT_DELAY_MS = 30000UL;

// Forward declarations of global accessor and bridge functions.
// Mode subclass .cpp files call these as free functions — they all delegate
// to the single AlarmController instance defined further below.
SensorManager&   getSensorManager();
ActuatorManager& getActuatorManager();
LogManager&      getLogManager();
DisplayManager&  getDisplayManager();
void             requestAuthentication();
void             sensorTriggerManager();


class AlarmController {
private:
    // Current active mode (Idle, Armed, Alarm, etc.)
    Mode* currentMode;

    // Hardware manager objects — own all actuator and sensor logic
    Buzzer          buzzer;
    LED             statusLED;
    SolenoidLock    solenoid;
    ActuatorManager actuatorManager;

    SensorConfig   sensorCfg;
    PIRSensor      pirSensor;
    MagneticSensor magSensor;
    SensorManager  sensorManager;

    // Authentication and facial recognition
    AuthenticationManager authenticationManager;
    FacialRecognition     facialRec;

    // Mode objects — each encapsulates behaviour for one system state
    CountdownMode countdownMode;
    IdleMode      idleMode;
    ArmedMode     armedMode;
    AlarmMode     alarmMode;

    // Logging and display output
    LogManager     logManager;
    DisplayManager displayManager;

    // Current state of the system state machine
    SysState sysState;

    // ENTRY_DELAY_MS stays private — only the controller uses it
    unsigned long ENTRY_DELAY_MS;

    // Fixed durations for alarm and cooldown phases
    const unsigned long ALARM_DURATION  = 20000UL;
    const unsigned long COOLDOWN_PERIOD = 20000UL;

    // Timestamps used to track when each timed phase began
    unsigned long exitDelayStart;
    unsigned long entryDelayStart;
    unsigned long alarmStart;
    unsigned long cooldownStart;
    unsigned long lastBeep;

    // Timestamp recorded when the controller was first initialised
    int systemBeginTime;

public:
    // Constructor — initialises all hardware objects and sets the system to idle.
    // Member initialiser list is used to pass constructor arguments to objects
    // that cannot be default-constructed (e.g. Buzzer, PIRSensor).
    AlarmController()
        : currentMode(nullptr),
          buzzer(BUZZER_PIN, 4000),
          statusLED(LED_PIN, "Red"),
          solenoid(SOLENOID_PIN),
          actuatorManager(buzzer, statusLED, solenoid),
          pirSensor("PIR1", "PIR",  sensorCfg, PIR_PIN, 0.5, 5.0),
          magSensor("MAG1", "Door", sensorCfg, MAG_PIN, 0.5, "NC"),
          facialRec(3000UL),
          sysState(SYS_IDLE),
          ENTRY_DELAY_MS(15000UL),
          exitDelayStart(0),
          entryDelayStart(0),
          alarmStart(0),
          cooldownStart(0),
          lastBeep(0),
          systemBeginTime(0)
    {}

    // Initialises serial, pins, sensors, and sets the starting mode.
    // Called once from Arduino's setup() entry point.
    void setup() {
        Serial.begin(9600);

        // Set pin modes explicitly — do not rely solely on class calibrate()
        pinMode(PIR_PIN, INPUT);
        pinMode(MAG_PIN, INPUT_PULLUP);

        // Register both sensors with the sensor manager
        sensorManager.addSensor(&pirSensor);
        sensorManager.addSensor(&magSensor);

        pirSensor.calibrate();
        magSensor.calibrate();

        // Record when the system first came online
        systemBeginTime = (int)millis();

        setMode(&idleMode);
        sysState = SYS_IDLE;

        logManager.record("SYSTEM READY");
        displayManager.setbuffer("SYSTEM READY");
        Serial.println("SYSTEM READY");
    }

    // Parses and dispatches a single serial command string received from MATLAB.
    // Placed before update() so that update() can call it without a forward declaration.
    void handleCommand(const String& cmd) {

        if (cmd == "ARM") {
            // Only allow arming from the idle state
            if (sysState != SYS_IDLE) return;

            Serial.print("USING_EXIT_DELAY_MS:");
            Serial.println(EXIT_DELAY_MS);

            sysState       = SYS_EXIT_DELAY;
            exitDelayStart = millis();
            lastBeep       = 0;

            logManager.record("ARM commanded");
            displayManager.setbuffer("EXIT DELAY");
            Serial.println("STATE: EXIT_DELAY");
        }

        else if (cmd == "DISARM") {
            countdownMode.setCountdownMode(false);
            sysState = SYS_IDLE;
            setMode(&idleMode);
            logManager.record("DISARM commanded");
            displayManager.setbuffer("DISARMED");
            Serial.println("STATE: IDLE");
        }

        // Raw digitalRead bypasses the class wrapper for a guaranteed fresh reading
        else if (cmd == "CHECK_DOOR") {
            int mag = digitalRead(MAG_PIN);
            Serial.println(mag == MAG_OPEN_STATE ? "MAG_OPEN" : "MAG_CLOSED");
        }

        // Prints full system state including raw MAG value for polarity verification
        else if (cmd == "STATUS") {
            int pir = digitalRead(PIR_PIN);
            int mag = digitalRead(MAG_PIN);

            const char* stateStr = "UNKNOWN";
            switch (sysState) {
                case SYS_IDLE:        stateStr = "IDLE";        break;
                case SYS_EXIT_DELAY:  stateStr = "EXIT_DELAY";  break;
                case SYS_ARMED:       stateStr = "ARMED";       break;
                case SYS_ENTRY_DELAY: stateStr = "ENTRY_DELAY"; break;
                case SYS_ALARM:       stateStr = "ALARM";       break;
                case SYS_COOLDOWN:    stateStr = "COOLDOWN";    break;
            }

            Serial.print("STATE:");           Serial.print(stateStr);
            Serial.print(",PIR:");            Serial.print(pir == HIGH ? "MOTION" : "CLEAR");
            Serial.print(",MAG:");            Serial.print(mag == MAG_OPEN_STATE ? "OPEN" : "CLOSED");
            Serial.print(",RAW_MAG:");        Serial.print(mag);
            Serial.print(",MAG_OPEN_STATE:"); Serial.println(MAG_OPEN_STATE);
        }

        // Raw digitalRead so open/close reflects immediately in the MATLAB sensor test panel
        else if (cmd == "TEST_SENSOR") {
            int pir = digitalRead(PIR_PIN);
            int mag = digitalRead(MAG_PIN);
            Serial.print("PIR:");
            Serial.print(pir == HIGH ? "MOTION" : "CLEAR");
            Serial.print(",MAG:");
            Serial.println(mag == MAG_OPEN_STATE ? "OPEN" : "CLOSED");
        }

        // Runs a quick cycle on all actuators to confirm hardware is working
        else if (cmd == "TEST_ACTUATOR") {
            Serial.println("ACTUATOR_TEST_START");
            actuatorManager.lockDoor();   delay(800);
            actuatorManager.unlockDoor(); delay(800);
            buzzer.beep(200);
            statusLED.flash(100);         delay(800);
            actuatorManager.stopAlarm();
            statusLED.deactivate();
            Serial.println("ACTUATOR_TEST_DONE");
        }

        // Triggers a lockout beep and display message after 3 failed attempts
        else if (cmd == "LOCKOUT") {
            buzzer.beep(500);
            statusLED.activate();
            delay(300);
            statusLED.deactivate();
            logManager.record("LOCKOUT — 3 failed attempts");
            displayManager.setbuffer("LOCKED OUT");
            Serial.println("LOCKOUT_OK");
        }

        // Dynamically sets the exit delay duration in seconds.
        // Updates the global EXIT_DELAY_MS so ArmedMode.cpp stays in sync.
        else if (cmd.startsWith("SET_EXIT:")) {
            int val = cmd.substring(9).toInt();
            if (val > 0) {
                EXIT_DELAY_MS = (unsigned long)val * 1000UL;
                logManager.record("EXIT_DELAY set to " + String(val) + "s");
                Serial.print("EXIT_DELAY_SET_MS:");
                Serial.println(EXIT_DELAY_MS);
            }
        }

        // Dynamically sets the entry delay duration in seconds
        else if (cmd.startsWith("SET_ENTRY:")) {
            int val = cmd.substring(10).toInt();
            if (val > 0) {
                ENTRY_DELAY_MS = (unsigned long)val * 1000UL;
                logManager.record("ENTRY_DELAY set to " + String(val) + "s");
                Serial.print("ENTRY_DELAY_SET_MS:");
                Serial.println(ENTRY_DELAY_MS);
            }
        }

        // Handles PIN (and optionally face) authentication from MATLAB.
        // Format: "pin:<digits>" or "pin:<digits>:face_ok"
        else if (cmd.startsWith("pin:")) {
            int sep = cmd.indexOf(':', 4);

            String inputPin;
            bool faceOk = false;

            if (sep != -1) {
                // Face result was sent inline with the PIN
                inputPin = cmd.substring(4, sep);
                faceOk   = (cmd.substring(sep + 1) == "face_ok");
            } else {
                // No inline face result — use whatever facialRec last detected
                inputPin = cmd.substring(4);
                faceOk   = facialRec.isFaceDetected();
            }

            if (authenticationManager.isLockedOut()) {
                Serial.println("AUTH_RESULT:LOCKED");
                return;
            }

            bool granted = authenticationManager.authenticate(inputPin, faceOk);
            if (granted) {
                authenticationSuccess();
            } else {
                authenticationFailure();
            }
        }

        // Receives face detection results from the MATLAB facial recognition pipeline
        else if (cmd == "face:ok") {
            facialRec.processMessage("FACE_OK");
        }
        else if (cmd == "face:fail") {
            facialRec.processMessage("FACE_FAIL");
        }

        // Switches authentication mode between pin, face, or both
        else if (cmd.startsWith("AUTH_MODE:")) {
            String mode = cmd.substring(10);
            mode.toLowerCase();
            authenticationManager.authenticationSelect(mode);
            authenticationManager.displayMode();
        }

        // Updates the stored PIN at runtime
        else if (cmd.startsWith("SET_PIN:")) {
            String newPin = cmd.substring(8);
            authenticationManager.setPin(newPin);
            logManager.record("PIN updated");
            Serial.println("PIN_UPDATED");
        }
    }

    // Main update loop — called repeatedly from Arduino's loop() entry point.
    // Handles face recognition timeout, display updates, serial commands,
    // and advances the state machine.
    void update() {
        // Keep face-recognition window alive or expire it
        facialRec.update();

        // Auto-clear the display after its timeout
        displayManager.update();

        // Process all queued serial commands before advancing the state machine
        while (Serial.available()) {
            String cmd = Serial.readStringUntil('\n');
            cmd.trim();
            if (cmd.length() > 0) handleCommand(cmd);
        }

        // Advance the state machine based on the current state
        switch (sysState) {

            case SYS_EXIT_DELAY:
                // Beep every 5 seconds as a departure warning
                if (millis() - lastBeep > 5000) {
                    lastBeep = millis();
                    buzzer.beep(100);
                }
                // Transition to ARMED once the exit delay has elapsed
                if (millis() - exitDelayStart >= EXIT_DELAY_MS) {
                    sysState = SYS_ARMED;
                    setMode(&armedMode);
                    logManager.record("STATE: ARMED");
                    displayManager.setbuffer("ARMED");
                    Serial.println("STATE: ARMED");
                }
                break;

            case SYS_ARMED: {
                int pir = digitalRead(PIR_PIN);
                int mag = digitalRead(MAG_PIN);

                // Any sensor trigger while armed immediately starts the entry delay
                if (pir == HIGH || mag == MAG_OPEN_STATE) {
                    sensorTriggerManager();
                    break;
                }

                currentMode->update();
                break;
            }

            case SYS_ENTRY_DELAY:
                // Fast beep every second as an urgency cue to the user
                if (millis() - lastBeep > 1000) {
                    lastBeep = millis();
                    buzzer.beep(80);
                }
                countdownMode.update(0.03f);

                // If the user has not authenticated in time, trigger the alarm
                if (millis() - entryDelayStart >= ENTRY_DELAY_MS) {
                    countdownMode.setCountdownMode(false);
                    sysState   = SYS_ALARM;
                    alarmStart = millis();
                    setMode(&alarmMode);
                    logManager.record("STATE: ALARM");
                    displayManager.setbuffer("ALARM!");
                    Serial.println("STATE: ALARM");
                    Serial.println("ALARM");
                }
                break;

            case SYS_ALARM:
                currentMode->update();

                // After the alarm duration, lock the door and enter cooldown
                if (millis() - alarmStart >= ALARM_DURATION) {
                    cooldownStart = millis();
                    setMode(&idleMode);
                    actuatorManager.lockDoor();
                    sysState = SYS_COOLDOWN;
                    logManager.record("STATE: COOLDOWN");
                    displayManager.setbuffer("COOLDOWN");
                    Serial.println("STATE: COOLDOWN");
                }
                break;

            case SYS_COOLDOWN:
                // After cooldown, automatically re-arm the system
                if (millis() - cooldownStart >= COOLDOWN_PERIOD) {
                    sysState = SYS_ARMED;
                    setMode(&armedMode);
                    logManager.record("STATE: ARMED (after cooldown)");
                    displayManager.setbuffer("ARMED");
                    Serial.println("STATE: ARMED (after cooldown)");
                }
                break;

            case SYS_IDLE:
            default:
                break;
        }
    }

    // Transitions from the current mode to a new mode, calling exit() and enter().
    void setMode(Mode* next) {
        if (currentMode) currentMode->exit();
        currentMode = next;
        if (currentMode) currentMode->enter();
    }

    // Returns the current mode pointer — matches UML's ModeStatus() : Mode*
    Mode* ModeStatus() {
        return currentMode;
    }

    // Sends an authentication request signal over serial to the MATLAB interface.
    void requestAuthentication() {
        Serial.println("AUTH_REQUEST");
    }

    // Called when any sensor fires while the system is ARMED.
    // Resets facial recognition and transitions to the entry delay phase.
    void sensorTriggerManager() {
        if (sysState != SYS_ARMED) return;

        // Reset face detection so a fresh scan is required during entry delay
        facialRec.reset();

        sysState        = SYS_ENTRY_DELAY;
        entryDelayStart = millis();
        lastBeep        = 0;

        countdownMode.setCountdownMode(true);
        Serial.println("STATE: ENTRY_DELAY (sensor)");
    }

    // Called on successful authentication — disarms the system and returns to idle.
    void authenticationSuccess() {
        logManager.recordAuth(true, "pin+face");
        displayManager.setbuffer("ACCESS GRANTED");
        Serial.println("AUTH_RESULT:OK");

        if (sysState != SYS_IDLE) {
            countdownMode.setCountdownMode(false);
            sysState = SYS_IDLE;
            setMode(&idleMode);
            Serial.println("STATE: IDLE");
        }
    }

    // Called on failed authentication — logs the attempt and checks for lockout.
    void authenticationFailure() {
        logManager.recordAuth(false, "pin+face");
        displayManager.setbuffer("ACCESS DENIED");
        Serial.println("AUTH_RESULT:FAIL");

        if (authenticationManager.isLockedOut()) {
            logManager.record("LOCKOUT triggered");
            Serial.println("LOCKOUT");
            buzzer.beep(500);
        }
    }

    // Public accessors so the global bridge functions below can reach
    // the private manager members of this class.
    SensorManager&   getSensorManager()   { return sensorManager;   }
    ActuatorManager& getActuatorManager() { return actuatorManager; }
    LogManager&      getLogManager()      { return logManager;      }
    DisplayManager&  getDisplayManager()  { return displayManager;  }
};


// Single global controller instance — the entire system lives here.
AlarmController controller;

// Global bridge implementations for manager accessors.
// Mode subclass .cpp files call these as free functions.
SensorManager&   getSensorManager()   { return controller.getSensorManager();   }
ActuatorManager& getActuatorManager() { return controller.getActuatorManager(); }
LogManager&      getLogManager()      { return controller.getLogManager();      }
DisplayManager&  getDisplayManager()  { return controller.getDisplayManager();  }

// Global bridge for requestAuthentication() — called by IdleMode, ArmedMode,
// and AlarmMode .cpp files as a free function.
void requestAuthentication() {
    controller.requestAuthentication();
}

// Global bridge for sensorTriggerManager() — called by ArmedMode.cpp
// as a free function.
void sensorTriggerManager() {
    controller.sensorTriggerManager();
}


// Arduino entry point — delegates entirely to the controller.
void setup() {
    controller.setup();
}

// Arduino main loop — delegates entirely to the controller.
void loop() {
    controller.update();
}