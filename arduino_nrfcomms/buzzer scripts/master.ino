#include <nrf_to_nrf.h>

nrf_to_nrf radio;
uint8_t hub_address[5] = {0x11, 0x22, 0x33, 0x44, 0x55};

const int RESET_PIN = 10;

const uint8_t MSG_TYPE_MASK = 0xF0;
const uint8_t MSG_ID_MASK   = 0x0F;
const uint8_t MSG_BUZZ      = 0x10;
const uint8_t MSG_WINNER    = 0x20;
const uint8_t MSG_RESET     = 0x30;
const uint8_t MSG_TEST_ALL  = 0x40;

bool round_locked = false;
uint8_t winner_id = 0;
bool last_reset_state = HIGH;
unsigned long reset_press_start = 0;
bool long_press_sent = false;
unsigned long last_test_broadcast = 0;
const unsigned long TEST_REPEAT_INTERVAL = 250;

uint8_t make_message(uint8_t type, uint8_t id) {
  return (type | (id & MSG_ID_MASK));
}

void broadcast_message(uint8_t type, uint8_t id, int repeats) {
  uint8_t msg = make_message(type, id);
  radio.stopListening();
  for (int i = 0; i < repeats; i++) {
    radio.write(&msg, 1);
    delay(4);
  }
  radio.startListening();
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(RESET_PIN, INPUT_PULLUP);

  Serial.println("\n--- MASTER MINIMAL ---");
  Serial.println("BOOT: master starting radio");

  radio.begin();
  radio.setChannel(7);
  radio.setPALevel(0);
  radio.setAutoAck(false);
  radio.setPayloadSize(1);
  radio.openReadingPipe(1, hub_address);
  radio.openWritingPipe(hub_address);
  radio.startListening();

  Serial.println("BOOT: master listening");
  Serial.println("Press reset button to clear round");
}

void loop() {
  bool current_reset_state = digitalRead(RESET_PIN);
  if (current_reset_state != last_reset_state) {
    delay(5);
    if (digitalRead(RESET_PIN) == LOW) {
      reset_press_start = millis();
      long_press_sent = false;
      Serial.println("BUTTON DOWN");
    } else {
      if (!long_press_sent) {
        round_locked = false;
        winner_id = 0;
        broadcast_message(MSG_RESET, 0, 4);
        Serial.println("ROUND RESET");
      } else {
        round_locked = false;
        winner_id = 0;
        broadcast_message(MSG_RESET, 0, 6);
        Serial.println("TEST ALL OFF");
      }
    }
    last_reset_state = current_reset_state;
  }

  if (digitalRead(RESET_PIN) == LOW && !long_press_sent && millis() - reset_press_start >= 2000) {
    long_press_sent = true;
    last_test_broadcast = millis();
    broadcast_message(MSG_TEST_ALL, 0, 6);
    Serial.println("TEST ALL SENT");
  }

  if (digitalRead(RESET_PIN) == LOW && long_press_sent && millis() - last_test_broadcast >= TEST_REPEAT_INTERVAL) {
    last_test_broadcast = millis();
    broadcast_message(MSG_TEST_ALL, 0, 2);
  }

  if (radio.available()) {
    uint8_t msg = 0;
    radio.read(&msg, 1);

    uint8_t type = msg & MSG_TYPE_MASK;
    uint8_t id = msg & MSG_ID_MASK;

    if (!round_locked && !long_press_sent && type == MSG_BUZZ && id > 0) {
      round_locked = true;
      winner_id = id;

      Serial.print("WINNER: ");
      Serial.println(winner_id);

      broadcast_message(MSG_WINNER, winner_id, 5);
    }
  }
}