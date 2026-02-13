/*
 * Simple LED Test for XIAO ESP32C3
 * 
 * This test verifies that:
 * 1. GPIO4 (D2) is working correctly
 * 2. Basic timing is accurate
 * 3. Hardware is properly connected
 * 
 * Upload this sketch first to verify hardware before uploading the full DCF77 emulator
 */

#define TEST_PIN 4  // D2 on XIAO ESP32C3

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n=== XIAO ESP32C3 GPIO Test ===");
  Serial.println("Testing GPIO4 (D2)");
  Serial.println("Connect LED + 220Ω resistor between GPIO4 and GND");
  Serial.println("LED should blink once per second\n");
  
  pinMode(TEST_PIN, OUTPUT);
  digitalWrite(TEST_PIN, LOW);
}

void loop() {
  static unsigned long lastBlink = 0;
  static int count = 0;
  
  unsigned long now = millis();
  
  if (now - lastBlink >= 1000) {
    lastBlink = now;
    count++;
    
    // Toggle pin
    digitalWrite(TEST_PIN, HIGH);
    Serial.printf("[%d] Pin HIGH\n", count);
    delay(100);
    
    digitalWrite(TEST_PIN, LOW);
    Serial.printf("[%d] Pin LOW\n", count);
  }
}
