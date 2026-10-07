# Burglar Alarm System

Arduino-based door and motion alarm with a PIN/facial-authentication state machine. The system monitors a PIR sensor and a normally-closed reed switch, drives a buzzer, status LED, and solenoid-lock driver, and accepts control and authentication commands over USB serial.

The firmware in [`firmware/BurglarAlarm`](firmware/BurglarAlarm) is the latest local `BurglarAlarm` source. The `BurglarAlarm_copy_20261007210446` folder was content-identical, so the duplicate was not copied. The separate `Controller` sketch is an older variant.

## Features

- Idle, exit-delay, armed, entry-delay, alarm, and cooldown states
- PIR motion and door/reed-switch monitoring
- PIN, face, or combined authentication modes (combined mode is the default)
- Three failed PIN attempts trigger a 60-second lockout
- Configurable exit and entry delays over serial
- Event log in RAM and serial status/test commands
- 20-second alarm phase, with buzzer auto-stop protection in the alarm mode

## Quick start

1. Review the board and wiring assumptions in [Hardware and wiring](docs/hardware.md).
2. Open `firmware/BurglarAlarm/BurglarAlarm.ino` in Arduino IDE.
3. Select the board and port that match your hardware, then compile and upload.
4. Open Serial Monitor at **9600 baud** with a newline line ending.
5. Send `STATUS` and test the sensors and actuators before arming.

The project uses Arduino core APIs and has no third-party library includes. The host-side facial-recognition pipeline is external; see [Serial protocol](docs/serial-protocol.md).

With Arduino CLI, provide the FQBN for your selected board:

```sh
arduino-cli compile --fqbn <board-fqbn> firmware/BurglarAlarm
arduino-cli upload --fqbn <board-fqbn> --port <serial-port> firmware/BurglarAlarm
```

## Documentation

- [Hardware and wiring](docs/hardware.md)
- [Software architecture and timing](docs/architecture.md)
- [Serial command protocol](docs/serial-protocol.md)

## Security and deployment limits

The default PIN (`1234`) is compiled into the firmware. `SET_PIN` changes it only in RAM, so it returns to the default after reset. Serial control and face-result commands are unauthenticated; anyone with access to the serial connection can issue commands, and face-only mode trusts the `face:ok` message. This is a prototype and must not be used as a production security system without persistent credential storage, authenticated communications, and a security review.
