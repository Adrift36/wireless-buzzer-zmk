#include <nrf_to_nrf.h>

nrf_to_nrf radio;
// Must match the transmitter exactly
uint8_t test_address[5] = {0x11, 0x22, 0x33, 0x44, 0x55}; 

void setup() {
  Serial.begin(115200);
  
  if (!radio.begin()) {
    Serial.println("RADIO HARDWARE FAILED TO INITIALIZE!");
    while (1); 
  }
  
  radio.setPALevel(0);
  radio.setChannel(7);
  radio.setAutoAck(false);
  radio.setPayloadSize(1);
  
  radio.openReadingPipe(1, test_address);
  radio.startListening(); // Put into Receive mode
  
  Serial.println("--- RECEIVER STARTING ---");
  Serial.println("Listening for pulses...");
}

void loop() {
  uint8_t pipeNo;
  // If we hear ANYTHING on the airwaves matching our address...
  if (radio.available(&pipeNo)) {
    uint8_t incoming_data = 0;
    radio.read(&incoming_data, 1);
    
    Serial.print("SUCCESS! Heard pulse containing: ");
    Serial.println(incoming_data);
  }
}