## Power & Ground Rails
All 5 V-powered devices (IR modules, LEDs, ultrasonic sensor, QTI modules, servo) share the Mega’s +5 V rail, and all grounds tie into the Mega’s GND bus. 
The nRF24L01+ radio, which requires 3.3 V, takes its VCC from the Mega’s 3.3 V pin (with a 10 µF decoupling capacitor nearby) and its GND to the same ground rail. 

## nRF24L01+ Wireless Transceiver
CE → Mega D48
CSN → Mega D49
MOSI → Mega D51
MISO → Mega D50
SCK → Mega D52
VCC → 3.3 V (not 5 V!)
GND → GND

This SPI-based link at up to 2 Mbps with auto-ACK enables bidirectional command/telemetry.

## IR Flame / Obstacle Detectors
Two HW-201 IR modules (“For Ladder” on the left and its twin on the right) each have VCC→5 V, GND→GND, and OUT → digital pins D35 and D36.
When the built-in IR photodiode sees a flame (or a close obstacle), the comparator output pulls LOW.

## Ladder Servo
Power (red) → 5 V rail
Ground (black) → GND rail
Signal (yellow) → Mega D13 (PWM)
The standard 3-wire RC servo uses a 50 Hz, 1–2 ms pulse to sweep 0–180°.

## Ultrasonic Sensor (HC-SR04)
VCC → 5 V
GND → GND
TRIG → D43
ECHO → D42
A 10 µs HIGH on TRIG emits 40 kHz pulses; ECHO goes HIGH for the echo-round-trip duration, converted to distance via distance = duration × 0.0343 / 2.

## QTI Reflectance Sensors
Two Pololu QTR-1A modules: each VCC→5 V, GND→GND, OUT → analog pins A2 and A3.
They output 0–5 V proportional to IR reflectance, ideal for line/boundary detection at a 3–6 mm stand-off.

## Indicator LEDs
IR Status LEDs: anodes to D36 (PC1) and D37 (PC0) via 220 Ω resistors; cathodes to GND.
Ultrasonic LEDs: anodes to D45 (PL4) and D47 (PL2) via 220 Ω resistors; cathodes to GND.
Port-manipulation macros in code (PORTC |= (1<<PC1), etc.) drive these almost instantaneously.

## Buzzer
+ → D53 (PB0/SS)
– → GND
A simple piezo/buzzer on a PWM-capable pin uses the Arduino tone() function to generate beeps and siren patterns
