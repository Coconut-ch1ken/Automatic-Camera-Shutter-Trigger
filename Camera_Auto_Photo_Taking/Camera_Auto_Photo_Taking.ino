#include <Servo.h>

// Servo and Button Setup
Servo myServo;
const int buttonPin = 13;  // Button connected to pin 13
const int servoPin = 11;   // Servo connected to pin 11
bool buttonState = HIGH;
bool lastButtonState = HIGH;
int photoTaken = 0;  // Counter for servo movements
bool isNight = false;

// Photoresistor Setup
const int photoResistorPin = A0; // Analog pin for photoresistor
const int lightThreshold = 200;  // Adjust based on your environment. 295 for large LDR, 350 for small LDR

// Timer Setup
unsigned long lastPhotoTime = 0;
// const unsigned long photoInterval = 1800000; // 30 minutes
const unsigned long photoInterval = 1798380; // 30 minutes

// 7-Segment Display Pin Mapping
const int segmentPins[] = {4, 0, 8, 5, 12, 3, 7, 9}; // a, b, c, d, e, f, g, dp
const int digitPins[] = {10, 2, 1, 6}; // D1, D2, D3, D4

// Segment values for digits 0-9 (Common Cathode)
const byte digits[10] = {
  0b0111111, // 0
  0b0000110, // 1
  0b1011011, // 2
  0b1001111, // 3
  0b1100110, // 4
  0b1101101, // 5
  0b1111101, // 6
  0b0000111, // 7
  0b1111111, // 8
  0b1101111  // 9
};

// Function to display a number (0-9999)
void displayNumber(int num) {
  int digitValues[4] = {
    (num / 1000) % 10,
    (num / 100) % 10,
    (num / 10) % 10,
    num % 10
  };

  for (int i = 0; i < 4; i++) {
    digitalWrite(digitPins[i], LOW);  // Activate digit
    displayDigit(digitValues[i]);     // Show corresponding number
    delay(1);                         // Short delay for persistence of vision
    digitalWrite(digitPins[i], HIGH); // Deactivate digit
  }
}

// Function to light up a single digit
void displayDigit(int num) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], (digits[num] >> i) & 0x01);
  }
}

void takePhoto() {
  // Move Servo
  myServo.write(0);
  delay(500);
  myServo.write(90);
  delay(500);
  myServo.write(0);
  delay(500);
  myServo.write(90);

  // Increment counter
  photoTaken++;
}

void setup() {
  // Initialize segment and digit pins
  for (int i = 0; i < 8; i++) {
    pinMode(segmentPins[i], OUTPUT);
    digitalWrite(segmentPins[i], LOW);
  }
  for (int i = 0; i < 4; i++) {
    pinMode(digitPins[i], OUTPUT);
    digitalWrite(digitPins[i], HIGH);
  }

  pinMode(buttonPin, INPUT_PULLUP); // Internal pull-up resistor
  myServo.attach(servoPin);
  myServo.write(90); // Initial servo position.

  takePhoto(); // Take initial photo
}

void loop() {
  buttonState = digitalRead(buttonPin);
  int lightLevel = analogRead(photoResistorPin);

  // Manual photo capture
  if (buttonState == LOW && lastButtonState == HIGH) {
    takePhoto();
  }
  lastButtonState = buttonState;

  // Timed photo logic
  if (millis() - lastPhotoTime > photoInterval) {
    if (lightLevel > lightThreshold) {
      isNight = false;
      takePhoto(); // Bright enough
    } else if (!isNight) { // Not bright enough but is not night yet
      takePhoto(); // First time it becomes dark
      isNight = true;
    }
    lastPhotoTime = millis();
  }

  displayNumber(photoTaken);

  // for ( int i=0 ; i < 10; i++ ){
  //   int num = i*1000 + i*100 + i*10 + i;
  //   for ( int l=0; l < 101; l++ ){
  //     displayNumber(int(num));
  //     delay(1);
  //   }
  // }
}