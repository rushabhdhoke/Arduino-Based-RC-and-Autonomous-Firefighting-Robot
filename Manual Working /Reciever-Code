#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

// —————————————————————————————————————————————
//  nRF24L01 on Mega: CE = pin 48, CSN = pin 49
// —————————————————————————————————————————————
#define CE_PIN   48
#define CSN_PIN  49
RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

// —————————————————————————————————————————————
//  Motor & speed pins
// —————————————————————————————————————————————
#define SPEED           100
#define TURN_SPEED      100

// Front wheels
#define speedPinR          9
#define RightMotorDirPin1 22
#define RightMotorDirPin2 24
#define speedPinL         10
#define LeftMotorDirPin1  26
#define LeftMotorDirPin2  28

// Rear wheels
#define speedPinRB         11
#define RightMotorDirPin1B  5
#define RightMotorDirPin2B  6
#define speedPinLB         12
#define LeftMotorDirPin1B   7
#define LeftMotorDirPin2B   8

// —————————————————————————————————————————————
//  Ladder servo pins & state
// —————————————————————————————————————————————
const int  SERVO_PIN       = 13;           // PWM pin for servo
Servo      ladderServo;
bool       ladderDown      = false;
unsigned long ladderStart  = 0;
const    unsigned long LADDER_HOLD_MS = 2000;  // 2 s hold

// —————————————————————————————————————————————
//  Low‐level motor helpers
// —————————————————————————————————————————————
void FR_fwd(int s){ digitalWrite(RightMotorDirPin1, LOW);  digitalWrite(RightMotorDirPin2, HIGH); analogWrite(speedPinR, s); }
void FR_bck(int s){ digitalWrite(RightMotorDirPin1, HIGH); digitalWrite(RightMotorDirPin2, LOW);  analogWrite(speedPinR, s); }
void FL_fwd(int s){ digitalWrite(LeftMotorDirPin1,  LOW);  digitalWrite(LeftMotorDirPin2,  HIGH); analogWrite(speedPinL, s); }
void FL_bck(int s){ digitalWrite(LeftMotorDirPin1,  HIGH); digitalWrite(LeftMotorDirPin2,  LOW);  analogWrite(speedPinL, s); }
void RR_fwd(int s){ digitalWrite(RightMotorDirPin1B, LOW);  digitalWrite(RightMotorDirPin2B, HIGH); analogWrite(speedPinRB, s); }
void RR_bck(int s){ digitalWrite(RightMotorDirPin1B, HIGH); digitalWrite(RightMotorDirPin2B, LOW);  analogWrite(speedPinRB, s); }
void RL_fwd(int s){ digitalWrite(LeftMotorDirPin1B,  LOW);  digitalWrite(LeftMotorDirPin2B,  HIGH); analogWrite(speedPinLB, s); }
void RL_bck(int s){ digitalWrite(LeftMotorDirPin1B,  HIGH); digitalWrite(LeftMotorDirPin2B,  LOW);  analogWrite(speedPinLB, s); }

// —————————————————————————————————————————————
//  High‐level moves
// —————————————————————————————————————————————
void go_advance(int v) { RL_fwd(v); RR_fwd(v); FR_fwd(v); FL_fwd(v); }
void go_back(int v)    { RL_bck(v); RR_bck(v); FR_bck(v); FL_bck(v); }
void left_turn(int v)  { FL_bck(v); RL_bck(v); FR_fwd(v); RR_fwd(v); }
void right_turn(int v) { FL_fwd(v); RL_fwd(v); FR_bck(v); RR_bck(v); }
void stop_Stop() { 
  analogWrite(speedPinLB, 0);
  analogWrite(speedPinRB, 0);
  analogWrite(speedPinL,  0);
  analogWrite(speedPinR,  0);
}

// —————————————————————————————————————————————
//  GPIO init
// —————————————————————————————————————————————
void init_GPIO() {
  pinMode(RightMotorDirPin1, OUTPUT);
  pinMode(RightMotorDirPin2, OUTPUT);
  pinMode(speedPinR,        OUTPUT);
  pinMode(LeftMotorDirPin1, OUTPUT);
  pinMode(LeftMotorDirPin2, OUTPUT);
  pinMode(speedPinL,        OUTPUT);
  pinMode(RightMotorDirPin1B, OUTPUT);
  pinMode(RightMotorDirPin2B, OUTPUT);
  pinMode(speedPinRB,         OUTPUT);
  pinMode(LeftMotorDirPin1B,  OUTPUT);
  pinMode(LeftMotorDirPin2B,  OUTPUT);
  pinMode(speedPinLB,         OUTPUT);
  stop_Stop();
}

// —————————————————————————————————————————————
//  RF handshake + timeout
// —————————————————————————————————————————————
const unsigned long TIMEOUT_MS = 100;
unsigned long lastRxTime = 0;
bool dataAvailableAndRead(char &out) {
  if (!radio.available()) return false;
  radio.read(&out, 1);
  lastRxTime = millis();
  return true;
}

void setup() {
  Serial.begin(9600);
  init_GPIO();

  // Servo
  ladderServo.attach(SERVO_PIN);
  ladderServo.write(0);

  // Radio
  radio.begin();
  radio.setPayloadSize(1);
  radio.enableAckPayload();
  radio.openReadingPipe(1, address);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  lastRxTime = millis();

  Serial.println(F("Receiver up and running."));
}

void loop() {
  // —— 1) RF packet handling ——
  char cmd;
  if (dataAvailableAndRead(cmd)) {
    // 1a) Ping?
    if (cmd == 'P') {
      char ack = 1;
      radio.writeAckPayload(1, &ack, 1);
    }
    // 1b) Drive or ladder‐trigger
    else {
      switch (cmd) {
        case 'W': go_advance(SPEED);    break;
        case 'S': go_back(SPEED);       break;
        case 'A': left_turn(TURN_SPEED);break;
        case 'D': right_turn(TURN_SPEED);break;
        case 'X': stop_Stop();          break;
        case 'L':                       // ladder trigger
          if (!ladderDown) {
            ladderDown  = true;
            ladderStart = millis();
            ladderServo.write(180);
          }
          break;
        default:
          stop_Stop();
      }
    }
  }

  // —— 2) Timeout failsafe ——
  if (millis() - lastRxTime > TIMEOUT_MS) {
    stop_Stop();
  }

  // —— 3) Ladder state machine ——
  if (ladderDown && millis() - ladderStart >= LADDER_HOLD_MS) {
    ladderServo.write(0);
    ladderDown = false;
  }
}

