# Software architecture

## Runtime flow

`BurglarAlarm.ino` owns the `AlarmController`. Arduino `setup()` initializes serial and sensors, registers the PIR and magnetic sensor, then enters Idle. The Arduino `loop()` delegates to the controller's update method, which processes serial input and advances the state machine.

| State | Behavior |
| --- | --- |
| Idle | System is disarmed and waiting for `ARM`. |
| Exit delay | Defaults to 30 seconds; beeps every five seconds before arming. |
| Armed | PIR motion or an open/tampered reed circuit starts the entry delay. |
| Entry delay | Defaults to 15 seconds; periodic beeps allow time to authenticate. |
| Alarm | Runs for 20 seconds, then moves to cooldown. |
| Cooldown | Holds the post-alarm state for 20 seconds before returning to Idle. |

Three failed PIN attempts cause a 60-second authentication lockout. `SET_EXIT:<seconds>` and `SET_ENTRY:<seconds>` change the delays at runtime; those settings are not persistent.

## Modules

| Files | Responsibility |
| --- | --- |
| `BurglarAlarm.ino` | Hardware composition, command handling, state machine, and Arduino entry points. |
| `Sensor.*`, `PIRSensor.*`, `MagneticSensor.*`, `SensorConfig.*`, `SensorManager.*` | Sensor lifecycle, polling, and status. |
| `Actuator.*`, `Buzzer.*`, `LED.*`, `SolenoidLock.*`, `ActuatorManager.*` | Hardware output control. |
| `AuthenticationManager.*`, `PinAuthenticator.*`, `FacialRecognition.*` | PIN and face-result authentication. |
| `Mode.*`, `IdleMode.*`, `ArmedMode.*`, `CountdownMode.*`, `AlarmMode.*`, `CountdownTimer.*` | State-specific behavior and countdown display support. |
| `DisplayManager.*`, `LogManager.*`, `AudioManager.*` | Display buffer, in-memory event log, and short audio feedback. |

`DisplayManager` currently reports its buffer over serial as a stand-in for a physical LCD. The event log is held in RAM and is lost on reset.
