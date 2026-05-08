#include <nrf_to_nrf.h>

nrf_to_nrf radio;
// Explicit 5-byte hex address. No strings, no terminators.
uint8_t test_address[5] = {0x11, 0x22, 0x33, 0x44, 0x55}; 
uint8_t payload = 42;

void setup() {
  Serial.begin(115200);
  
  if (!radio.begin()) {
    Serial.println("RADIO HARDWARE FAILED TO INITIALIZE!");
    while (1); // Freeze forever
  }
  
  radio.setPALevel(0);
  radio.setChannel(7);
  radio.setAutoAck(false);
  radio.setPayloadSize(1);
  
  radio.openWritingPipe(test_address);
  radio.stopListening(); // Put into Transmit mode
  
  Serial.println("--- TRANSMITTER STARTING ---");
}

void loop() {
  Serial.println("Transmitting pulse...");
  radio.write(&payload, 1);
  delay(1000); // Wait 1 second
}