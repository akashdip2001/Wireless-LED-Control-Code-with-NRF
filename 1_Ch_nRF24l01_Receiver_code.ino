/*
 * This is a 1 channel nrf24l01 transmitter and receiver code.
 * There is a single output(led control) in the receiver which can controlled with the pushbutton in transmitter; 
 */


#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

RF24 radio(7,8); // declaring CE and CSN pins
const byte address[] = "node1";

bool buttonState = 0; // stores the received data of state of button after

void setup() {
  radio.begin(); // initializes the operations of the chip
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  
  pinMode(3, OUTPUT); // declares LEDpin as an output
  
}


void loop() {
  while(radio.available()){  
    radio.read(&buttonState, sizeof(buttonState));
    digitalWrite(3, buttonState);
  }

}
