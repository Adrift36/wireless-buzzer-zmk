#include <nrf_to_nrf.h>

const int BUTTON_PIN = 10;
const int LED_PIN = 7;

// ---> CHANGE THIS FOR EACH PLAYER BOARD <---
uint8_t MY_BUZZER_ID = 3; 

nrf_to_nrf radio;
uint8_t hub_address[5]    = {0x11, 0x22, 0x33, 0x44, 0x55}; 
uint8_t beacon_address[5] = {0x66, 0x77, 0x88, 0x99, 0xAA}; 
#include <nrf_to_nrf.h>

const int BUTTON_PIN = 10;
const int LED_PIN = 7;

nrf_to_nrf radio;
uint8_t hub_address[5] = {0x11, 0x22, 0x33, 0x44, 0x55};
uint8_t my_id = 3; // Change this for each slave (1, 2, 3, etc)

const uint8_t MSG_TYPE_MASK = 0xF0;
const uint8_t MSG_ID_MASK   = 0x0F;
const uint8_t MSG_BUZZ      = 0x10;
const uint8_t MSG_WINNER    = 0x20;
const uint8_t MSG_RESET     = 0x30;
const uint8_t MSG_TEST_ALL  = 0x40;

bool last_button_state = HIGH;
bool round_open = true;

uint8_t make_message(uint8_t type, uint8_t id) {
  return (type | (id & MSG_ID_MASK));
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.print("\n--- SLAVE MINIMAL (ID: ");
  Serial.print(my_id);
  Serial.println(") ---");

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.print("BOOT: button state = ");
  Serial.println(digitalRead(BUTTON_PIN));
  Serial.println("BOOT: slave starting radio");

  radio.begin();
  radio.setChannel(7);
  radio.setPALevel(0);
  radio.setAutoAck(false);
  radio.setPayloadSize(1);
  radio.openWritingPipe(hub_address);
  radio.openReadingPipe(1, hub_address);
  radio.startListening();

  Serial.println("BOOT: slave ready");
}

void loop() {
  if (radio.available()) {
    uint8_t msg = 0;
    radio.read(&msg, 1);

    uint8_t type = msg & MSG_TYPE_MASK;
    uint8_t id = msg & MSG_ID_MASK;

    if (type == MSG_WINNER) {
      round_open = false;
      if (id == my_id) {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("I WON");
      } else {
        digitalWrite(LED_PIN, LOW);
        Serial.print("Winner is ");
        Serial.println(id);
      }
    } else if (type == MSG_RESET) {
      round_open = true;
      digitalWrite(LED_PIN, LOW);
      Serial.println("ROUND OPEN");
    } else if (type == MSG_TEST_ALL) {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("TEST ALL ON");
    }
  }

  bool current_button_state = digitalRead(BUTTON_PIN);

  if (current_button_state != last_button_state) {
    if (current_button_state == LOW) {
      if (round_open) {
        uint8_t msg = make_message(MSG_BUZZ, my_id);

        Serial.print("Button pressed, buzzing ID: ");
        Serial.println(my_id);

        radio.stopListening();
        bool sent = radio.write(&msg, 1);
        radio.startListening();

        Serial.print("Write result: ");
        Serial.println(sent);
      } else {
        Serial.println("Button ignored: round locked");
      }

      unsigned long start_wait = millis();
      while (millis() - start_wait < 250) {
        if (radio.available()) {
          uint8_t reply = 0;
          radio.read(&reply, 1);
          Serial.print("REPLY: ");
          Serial.println(reply);
          break;
        }
      }
    } else {
      Serial.println("Button released");
    }

    delay(25);
    last_button_state = current_button_state;
  }
}