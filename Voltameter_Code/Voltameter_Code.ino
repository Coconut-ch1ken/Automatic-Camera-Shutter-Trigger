    //  VIN (to measure)
    //     |
    //     |
    //    [ R1 ]  ← 100kΩ
    //     |
    //     +------> A0 (Analog pin)
    //     |
    //    [ R2 ]  ← 10kΩ
    //     |
    //    GND

// ^ This is how you should wire the stuff in order for this code to work properly


#include <Arduino.h>

// 7-Segment Display Setup
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

// Voltage divider and analog pin setup
const int voltagePin = A0;
const float voltageDividerRatio = 11.0;    // adjust if you use a different divider
const float referenceVoltage = 5.0;        // Assuming Arduino runs on 5V USB

void setup() {
  // Setup segment pins
  for (int i = 0; i < 8; i++) {
    pinMode(segmentPins[i], OUTPUT);
    digitalWrite(segmentPins[i], LOW);
  }

  // Setup digit control pins
  for (int i = 0; i < 4; i++) {
    pinMode(digitPins[i], OUTPUT);
    digitalWrite(digitPins[i], HIGH); // Common cathode, so HIGH = off
  }
}

// Read actual VIN voltage using voltage divider
float readVoltage() {
  int analogValue = analogRead(voltagePin);
  float voltageAtA0 = (analogValue / 1023.0) * referenceVoltage;
  return voltageAtA0 * voltageDividerRatio;
}

// Display a single digit (0–9)
void displayDigit(int num, bool showDot = false) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], (digits[num] >> i) & 0x01);
  }
  digitalWrite(segmentPins[7], showDot ? HIGH : LOW); // Dot point
}

// Show a float voltage value in format XX.X
void displayVoltage(float voltage) {
  int displayVal = int(voltage * 10);  // e.g. 12.3 → 123

  int digitValues[4];
  digitValues[0] = (displayVal / 1000) % 10;
  digitValues[1] = (displayVal / 100) % 10;
  digitValues[2] = (displayVal / 10) % 10;
  digitValues[3] = displayVal % 10;

  for (int i = 0; i < 4; i++) {
    digitalWrite(digitPins[i], LOW); // Enable current digit
    displayDigit(digitValues[i], i == 1); // Show dot after second digit
    delay(1);
    digitalWrite(digitPins[i], HIGH); // Turn off digit after display
  }
}

void loop() {
  float voltage = readVoltage();
  for (int i = 0; i < 100; i++) {
    displayVoltage(voltage);
  }
}

