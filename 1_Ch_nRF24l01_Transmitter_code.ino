/*
 * This is a 1 channel nrf24l01 transmitter and receiver code.
 * There is a single input(push button) in the transmitter which can control the led in receiver;
 */


#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

RF24 radio(7,8);   // declaring CE and CSN pins
const byte address[] = "node1"; 

bool buttonCheck = 0;  // the value returned by digitalRead(4) is stored here

void setup() {
  radio.begin();  // initializes the operations of the chip
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();    
 
  pinMode(4, INPUT); // declares pushButton as an input

}
                                                                          

void loop() {
  buttonCheck = digitalRead(4);
  radio.write(&buttonCheck, sizeof(buttonCheck));
  
}
