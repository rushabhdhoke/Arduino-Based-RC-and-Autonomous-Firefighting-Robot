#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// nRF24L01 CE/CSN pins
#define CE_PIN     7
#define CSN_PIN    8

// Status LED (link indicator)
#define STATUS_LED 2 

// Button input pins (wired between pin and GND)
#define FORWARD_PIN   10
#define BACKWARD_PIN  11
#define LEFT_PIN      12
#define RIGHT_PIN     13
#define ESTOP_PIN     3 
#define LADDER_PIN    4   // <— new!

// RF24 object & address
RF24 radio(CE_PIN, CSN_PIN);

const byte address[6] = "00001";

// How often to ping the receiver (ms)
const unsigned long CHECK_INTERVAL = 5000;
unsigned long lastCheck = 0;
bool connected = false;

// Send a single‐byte 'P' ping; if we get back an ack=1, link is up
bool checkConnection() {
  const char ping = 'P';
char ack = 0; 
  bool ok = radio.write(&ping, 1);
  if (ok && radio.isAckPayloadAvailable()) {
    radio.read(&ack, 1);
    if (ack == 1) {
      if (!connected) {
        connected = true;
        Serial.println(F(">> Receiver is ONLINE <<"));
      }
      return true;
    }
  }
  if (connected) {
    connected = false;
    Serial.println(F(">> Receiver LOST <<"));
  }
  return false;
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // LED for link status
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, LOW);

  // Configure buttons with pull-ups
  pinMode(FORWARD_PIN,  INPUT_PULLUP);
  pinMode(BACKWARD_PIN, INPUT_PULLUP);
  pinMode(LEFT_PIN,     INPUT_PULLUP);
  pinMode(RIGHT_PIN,    INPUT_PULLUP);
  pinMode(ESTOP_PIN,    INPUT_PULLUP);
  pinMode(LADDER_PIN,   INPUT_PULLUP);  // new

  // Initialize radio
  radio.begin();
  radio.enableAckPayload();        // allow receiver to ACK
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_MIN);
  radio.openWritingPipe(address);
  radio.stopListening();

  Serial.println(F("Transmitter ready."));
  Serial.println(F("Waiting for receiver..."));

  // Block until first successful handshake
  while (!checkConnection()) {
    delay(200);
    // blink LED while waiting
    digitalWrite(STATUS_LED, (millis() & 0x200) ? HIGH : LOW);
  }

  // Once paired, LED stays solid
  digitalWrite(STATUS_LED, HIGH);
  Serial.println(F("Paired! Button commands active."));
}

void loop() {
  unsigned long now = millis();
  // Periodically re‐check the link
  if (now - lastCheck > CHECK_INTERVAL) {
    lastCheck = now;
    checkConnection();
    digitalWrite(STATUS_LED, connected ? HIGH : LOW);
  }

  // Only read buttons if link is up
  if (!connected) return;

  // Read buttons (active LOW), ladder first
  char cmd = 0;
  if (digitalRead(ESTOP_PIN)  == LOW) cmd = 'X';  // emergency stop
  else if (digitalRead(LADDER_PIN) == LOW) cmd = 'L';  // <— ladder
  else if (digitalRead(FORWARD_PIN)== LOW) cmd = 'W';
  else if (digitalRead(BACKWARD_PIN)==LOW) cmd = 'S';
  else if (digitalRead(LEFT_PIN)   == LOW) cmd = 'A';
  else if (digitalRead(RIGHT_PIN)  == LOW) cmd = 'D';
  else return;  // no button pressed

  // Transmit the command byte
  bool ok = radio.write(&cmd, 1);
  if (ok && radio.isAckPayloadAvailable()) {
    char ack;
    radio.read(&ack, 1);
    Serial.print(F("Sent: "));
    Serial.println(cmd);
  } else {
    Serial.println(F("Transmit failed or no ACK"));
  }

  delay(100);  // debounce/pacing
}
