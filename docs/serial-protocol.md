# Serial protocol

The controller reads newline-terminated commands at **9600 baud**. Commands and replies are plain text. The host facial-recognition/MATLAB process is not included in this repository.

| Command | Purpose |
| --- | --- |
| `ARM` | Start the exit delay from Idle. |
| `DISARM` | Return the system to Idle. |
| `STATUS` | Report state and current PIR/reed-switch readings, including raw magnetic input. |
| `CHECK_DOOR` | Reply `MAG_OPEN` or `MAG_CLOSED`. |
| `TEST_SENSOR` | Report PIR and magnetic-switch readings. |
| `TEST_ACTUATOR` | Exercise lock, buzzer, and LED outputs. Use only during safe bring-up. |
| `LOCKOUT` | Manually show the lockout indication. |
| `SET_EXIT:<seconds>` | Set the exit delay when the value is positive. |
| `SET_ENTRY:<seconds>` | Set the entry delay when the value is positive. |
| `pin:<digits>` | Submit a PIN; in default `both` mode a recent/inline face result is also required. |
| `pin:<digits>:face_ok` | Submit a PIN and an inline positive face result. |
| `face:ok` / `face:fail` | Update the face-recognition result window. |
| `AUTH_MODE:pin`, `AUTH_MODE:face`, `AUTH_MODE:both` | Select authentication mode. |
| `SET_PIN:<digits>` | Replace the in-memory PIN until reset. |

Replies include status strings such as `STATE:ARMED`, `AUTH_RESULT:LOCKED`, `EXIT_DELAY_SET_MS:<ms>`, and `PIN_UPDATED`. The exact messages are emitted in `BurglarAlarm.ino` and the authentication modules.

## Trust boundary

There is no command authentication or message integrity check. Anyone who can write to the serial port can arm/disarm the system, alter timing, change the runtime PIN, and provide `face:ok`. Do not connect this protocol to an untrusted network or treat serial authentication as a security boundary.
