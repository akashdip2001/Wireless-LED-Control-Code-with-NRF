# Wireless LED Control Code with NRF

![NRF](https://github.com/user-attachments/assets/f48cd1ff-5484-4823-a58d-ae16e4f568ae)

<details>
  <summary style="opacity: 0.85;"><b>Old Code + connections + ✅ ARDUINO IDE setup</b></summary><br>
  
Here’s the **modified code** where pressing a switch on the **transmitter side** will make the **onboard LED (pin 13) of the receiver blink**.

The image contains the following components:

1. Two NRF24L01 wireless modules (with SMA antenna connectors).
2. Two external antennas (for NRF24L01 modules).
3. Two Arduino Nano boards (with USB-C ports).
4. Two NRF24L01 adapter modules (to regulate voltage and provide stable communication).
5. A power switch.

---

<p align="center">
  <img src="https://github.com/user-attachments/assets/9f9f3065-9689-4903-9115-2d919a661cc7" alt="Image 1" width="42%" style="margin-right: 10px;"/>
  <img src="https://github.com/user-attachments/assets/f6d41a19-6ad4-4d67-9c3e-62238c5789d3" alt="Image 2" width="56%" style="margin-right: 10px;"/>
</p>

---

### **Connections**

<img align="right" alt="servo" width="40%" src="https://github.com/user-attachments/assets/afed39f7-98da-4940-bdf9-67ab0c28900f">

#### **Transmitter (Arduino Nano + NRF24L01 + Switch)**
| NRF24L01 Pin | Arduino Nano Pin |
|-------------|----------------|
| VCC | 3.3V |
| GND | GND |
| CE | D9 |
| CSN | D10 |
| SCK | D13 |
| MOSI | D11 |
| MISO | D12 |

**Switch Connection (Transmitter Side)**
| Switch Pin | Arduino Nano Pin |
|-----------|-----------------|
| One Side | D2 |
| Other Side | GND |

---

**Onboard LED (Receiver Side)**
- The built-in **LED on Arduino Nano is on pin 13**.

---

![Screenshot (257)](https://github.com/user-attachments/assets/06fa3d60-073a-44b5-9853-f4784c6fe9ee)

## **Transmitter Code (Send Data on Switch Press)**
```cpp
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN pins
const byte address[6] = "00001"; // Communication address
const int switchPin = 2; // Switch connected to D2
bool switchState = false;

void setup() {
    Serial.begin(115200);
    pinMode(switchPin, INPUT_PULLUP); // Use internal pull-up resistor
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MIN);
    radio.stopListening(); // Set as transmitter
}

void loop() {
    switchState = digitalRead(switchPin) == LOW; // Active LOW switch
    radio.write(&switchState, sizeof(switchState));
    Serial.print("Switch State Sent: ");
    Serial.println(switchState);
    delay(100); // Debounce delay
}
```

---

#### **Receiver (Arduino Nano + NRF24L01 + LED)**
| NRF24L01 Pin | Arduino Nano Pin |
|-------------|----------------|
| VCC | 3.3V |
| GND | GND |
| CE | D9 |
| CSN | D10 |
| SCK | D13 |
| MOSI | D11 |
| MISO | D12 |

<p align="center">
  <img src="https://github.com/user-attachments/assets/f8355d64-f0fc-4260-b990-aa208b63ef81" alt="Image 1" width="46%" style="margin-right: 10px;"/>
  <img src="https://github.com/user-attachments/assets/27b2d67a-1e54-4d2d-b199-6ed9f9aef664" alt="Image 2" width="46%" style="margin-right: 10px;"/>
</p>

## **Receiver Code (Blink Onboard LED on Signal)**
```cpp
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN pins
const byte address[6] = "00001";
const int ledPin = 13; // Built-in LED

bool switchState = false;

void setup() {
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening(); // Set as receiver
}

void loop() {
    if (radio.available()) {
        radio.read(&switchState, sizeof(switchState));
        Serial.print("Received Switch State: ");
        Serial.println(switchState);

        if (switchState) {
            // Blink the onboard LED
            digitalWrite(ledPin, HIGH);
            delay(200);
            digitalWrite(ledPin, LOW);
            delay(200);
        } else {
            digitalWrite(ledPin, LOW); // Turn off LED if switch is not pressed
        }
    }
}
```

---

### **How It Works**
1. **On Transmitter Side:**
   - The switch is connected to **D2** with an **internal pull-up** resistor.
   - When **pressed**, it sends a signal (`true`).
   - When **released**, it sends (`false`).
  
2. **On Receiver Side:**
   - If it receives `true`, the onboard **LED (pin 13) blinks**.
   - If it receives `false`, the LED **turns off**.

---

### **Optimizations:**
✔ **Debounced input:** 100ms delay prevents multiple triggers.  
✔ **Power-efficient RF module settings:** Uses `RF24_PA_MIN`.  
✔ **No unnecessary delays in receiver** (only LED blink cycle).  

</details>

https://github.com/user-attachments/assets/263dbe1e-9529-4ec1-8dab-7897d9b616dc

## Transmitter code

```cpp
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
```

## Receiver code

```cpp
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
```
