# Hardware and wiring

## Board and pins

The sketch uses Arduino digital pin numbers and core APIs. Select a board that supports the configured pins, `tone()`, `INPUT_PULLUP`, and `Serial`; the source does not pin a specific board model or FQBN.

| Function | Pin | Notes |
| --- | ---: | --- |
| PIR sensor output | D5 | Digital HIGH indicates motion. |
| NC magnetic/reed switch | D6 | Wire the normally-closed contact between D6 and GND. With `INPUT_PULLUP`, closed reads LOW and an open door or cut wire reads HIGH. |
| Buzzer | D8 | Alarm buzzer; `AudioManager` also uses this pin for short tones. |
| Status LED | D9 | The current sketch labels this as the red status LED. Use a suitable resistor or driver. |
| Solenoid-lock control | D10 | Logic control only; drive the solenoid through a correctly rated transistor/MOSFET and flyback protection. |

The magnetic input is configured with `INPUT_PULLUP` by both the controller and the sensor calibration routine. Confirm the reed-switch contact type and polarity with `STATUS` before arming.

## Safe bring-up

1. Upload the sketch with the solenoid power stage disconnected.
2. Open Serial Monitor at 9600 baud and send `STATUS`.
3. Open and close the reed switch; confirm `MAG` changes between `OPEN` and `CLOSED`.
4. Trigger the PIR and confirm the reported state changes to `MOTION`.
5. Use `TEST_ACTUATOR` with the lock mechanism unloaded to verify output polarity.
6. Connect the lock through a separately powered, appropriately rated driver stage. Do not power a solenoid directly from an Arduino GPIO.

The firmware repository does not define mains wiring, battery backup, isolation, enclosure, or a specific board-level pinout. Validate those parts against the hardware actually used.
