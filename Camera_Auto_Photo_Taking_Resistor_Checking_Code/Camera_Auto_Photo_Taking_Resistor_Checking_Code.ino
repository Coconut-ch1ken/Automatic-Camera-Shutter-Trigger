const int LDR1Pin = A0;  // LDR1 connected to A0 (5V side)
const int LDR2Pin = A5;  // LDR2 connected to A5 (3.3V side)
const int fixedResistor1 = 100000; // 100kΩ for LDR1
const int fixedResistor2 = 10000;  // 10kΩ for LDR2

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Read analog values
  int ldr1Value = analogRead(LDR1Pin);
  int ldr2Value = analogRead(LDR2Pin);

  // Convert to resistance using voltage divider formula
  float ldr1Resistance = fixedResistor1 * ((1023.0f / ldr1Value) - 1);
  float ldr2Resistance = fixedResistor2 * ((1023.0 / ldr2Value) - 1);

  // Print results
  Serial.print("LDR1 Value: ");
  Serial.print(ldr1Value);
  Serial.print(" | LDR2 Value: ");
  Serial.print(ldr2Value);
  Serial.print(" | LDR1 Resistance: ");
  Serial.print(ldr1Resistance);
  Serial.print(" ohms | LDR2 Resistance: ");
  Serial.print(ldr2Resistance);
  Serial.println(" ohms");

  delay(1000);
}

// bright: ldr1 value > 900
// dark: ldr1 < 300