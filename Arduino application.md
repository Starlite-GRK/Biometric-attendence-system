# Biometric-attendence-system

**Objective:**
The objective of the project was to develop a Biometric Attendance system using the AS608 fingerprint sensor.

**Abstract:**
Some of the widely used attendance systems apart from manual recording include maintaining timesheets in word or excel, mechanized systems, biometrics, punch card, smart card, and proximity reader. What gives biometrics an upper hand over the other alternatives is that there is no possibility of a proxy attendance being given. Some of the most common biometric systems can work with fingerprint reading, facial recognition, retinal recognition, or a combination of them.

Fingerprint recognition is one of the most economical, fast, and easy ways therefore for a basic prototype of an attendance system this project incorporates an AS608 fingerprint sensor and Arduino UNO among other components. Few of the economical sensors we found were RT305 and GT511C3 but AS608 was chosen based on the availability and ease of coding with its library Adafruit. It can store 128 individual fingerprints and recognize a fingerprint in 1 second.

**Block Diagram:**

![bio block dia](https://user-images.githubusercontent.com/77966030/221339619-fddbeae4-a4c4-48cd-80f8-49ed6ea59dbc.png)

**Wiring connections**

**AS608 fingerprint sensor**

Both V+ pins are connected to the common power supply line on the breadboard which is powered by the 3.3V pin of the Arduino UNO.

Both GND are connected to the common ground on the breadboard which is powered by the GND pin of the Arduino UNO.

TX pin is connected to pin 2 of Arduino UNO.

RX pin is connected to pin 3 of Arduino UNO.

**LCD**

![LCD wiring bio](https://user-images.githubusercontent.com/77966030/221339750-c8a67394-2a08-46c7-9da7-b7b957b795ac.png)

**LEDs**

Each anode terminal of red, blue and green LEDs are connected to pin no. 5,6 and 7 of Arduino respectively and all the cathode terminals are connected to separate resistors whose other terminals are connected to common ground. 

**Potentiometer**

First pin: +ve common power supply line

Middle pin: V0 pin of LCD

Third pin: Grounded
