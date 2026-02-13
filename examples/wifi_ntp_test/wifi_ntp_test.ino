/*
 * WiFi and NTP Test for XIAO ESP32C3
 * 
 * This test verifies that:
 * 1. WiFi connection works
 * 2. NTP time synchronization works
 * 3. Time is correctly displayed
 * 
 * Copy config.h from parent directory or create one with your WiFi credentials
 */

#include <WiFi.h>
#include <time.h>

// WiFi Configuration - Replace with your credentials
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

// NTP Configuration
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;      // CET = UTC+1
const int daylightOffset_sec = 3600;  // CEST = UTC+2 in summer

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n=== WiFi and NTP Test ===");
  
  // Connect to WiFi
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✓ WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Signal strength (RSSI): ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println("\n✗ Failed to connect to WiFi");
    return;
  }
  
  // Configure time
  Serial.println("\nSynchronizing with NTP server...");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
  // Wait for time to be set
  int retry = 0;
  while (time(nullptr) < 100000 && retry < 10) {
    Serial.print(".");
    delay(1000);
    retry++;
  }
  
  if (retry < 10) {
    Serial.println("\n✓ NTP time synchronized!");
  } else {
    Serial.println("\n✗ Failed to synchronize NTP time");
    return;
  }
  
  // Print current time
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    Serial.println("\nCurrent local time:");
    Serial.printf("  Date: %04d-%02d-%02d\n", 
                  timeinfo.tm_year + 1900, 
                  timeinfo.tm_mon + 1, 
                  timeinfo.tm_mday);
    Serial.printf("  Time: %02d:%02d:%02d\n",
                  timeinfo.tm_hour,
                  timeinfo.tm_min,
                  timeinfo.tm_sec);
    Serial.printf("  Day of week: %d (1=Monday, 7=Sunday)\n",
                  (timeinfo.tm_wday == 0) ? 7 : timeinfo.tm_wday);
  } else {
    Serial.println("✗ Failed to get local time");
  }
  
  Serial.println("\n=== Test Complete ===");
  Serial.println("If all checks passed (✓), your WiFi and NTP configuration is correct!");
}

void loop() {
  static unsigned long lastUpdate = 0;
  
  if (millis() - lastUpdate >= 10000) {
    lastUpdate = millis();
    
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      Serial.printf("Current time: %02d:%02d:%02d\n",
                    timeinfo.tm_hour,
                    timeinfo.tm_min,
                    timeinfo.tm_sec);
    }
  }
}
