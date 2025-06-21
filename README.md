# Arduino-Based-RC-and-Autonomous-Firefighting-Robot

![Firefighting Robot](./assets/robot-demo.jpg)

## 🚒 Project Overview

This is a hybrid RC/autonomous firefighting platform built on an Arduino Mega 2560 and nRF24L01+ radios. It can be driven manually via wireless commands, or switch into semi-autonomous mode to detect and “extinguish” simulated fires with IR flame sensors and a deployable ladder mechanism. Obstacle avoidance, boundary sensing, and real-time feedback LEDs & buzzer make it a robust demo of small-scale robotics.

---

## 🔧 Features

- **Dual-mode control**:
  - **Manual**: WASD or button-panel RC via nRF24L01  
  - **Autonomous**: IR-guided fire seeking + boundary/obstacle avoidance  
- **Flame detection**: Two front-mounted IR flame sensors  
- **Ladder actuation**: Servo-powered, 0–180° sweep with auto-retract  
- **Obstacle avoidance**: HC-SR04 ultrasonic distance sensing  
- **Boundary detection**: QTI reflectance sensors for line/grid following  
- **Feedback**:  
  - LEDs for IR, ultrasonic & line detection  
  - Buzzer siren during motion  
- **Safety**: RF timeouts and obstacle blocks prevent runaway

---

## 🛠️ Hardware

| Component                  | Connection                   |
|----------------------------|------------------------------|
| Arduino Mega 2560          | —                            |
| nRF24L01+ CE / CSN / SPI   | D48 / D49 / D51-D50-D52      |
| IR Flame Sensors ×2        | D34, D35                     |
| HC-SR04 Ultrasonic         | TRIG → D43, ECHO → D42       |
| QTI Reflectance Sensors ×2 | A2, A3                       |
| Ladder Servo               | Signal → D13, VCC → 5 V, GND |
| Indicator LEDs             | D36, D37, D45, D47 (PORTC/PORTL) |
| Buzzer                     | D53 (PB0/SS)                 |
| Motor Drivers (L298N)      | PWM & DIR pins D5–D10, D22–D28 |

> **See** [`/docs/wiring.md`](docs/wiring.md) for a full Fritzing diagram and breadboard layout.

---

## 💾 Software

1. Clone this repo:  
   ```bash
   git clone https://github.com/rushabhdhoke/Arduino-Based-RC-and-Autonomous-Firefighting-Robot.git
   ```
2. Open Transmitter.ino / Receiver.ino in the Arduino IDE.
3. Install dependencies via Library Manager:
- RF24 by TMRh20
- Servo (built-in)
3. Select Arduino Mega 2560 and your COM port.
4. Upload Transmitter sketch to your RC remote board; Receiver to the robot.

## ▶️ Usage
Power on both boards.
Wait for the “PAIRED” serial message (or solid status LED).

### Manual mode:
Use WASD or button pad to drive; press L to deploy ladder.

### Autonomous mode (flip the AUTO_MODE define in code):
Robot scans for IR flame, computes bearing, and drives toward it.
Obstacle/barrier detection halts or reroutes as needed.

## 📂 Repo Structure
/
├── docs/            # Wiring diagrams & Fritzing files
├── assets/          # Photos, demo GIFs
├── Transmitter/     # Remote-control transmitter code
├── Receiver/        # Robot control & autonomous routines
└── README.md
