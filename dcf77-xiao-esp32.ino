/*
 * DCF77 Signal Emulator for Xiao ESP32 C3
 * 
 * This sketch emulates a DCF77 time signal transmitter for syncing Casio multiband watches.
 * It wakes up during Casio watch sync hours, fetches accurate time via NTP, and transmits
 * the DCF77 signal.
 * 
 * Hardware: Seeed Studio XIAO ESP32C3
 * Output Pin: D2 (GPIO4) - Connect to transistor base for signal amplification
 * 
 * Author: DCF77 Emulator Project
 * License: MIT
 */

#include <WiFi.h>
#include <time.h>
#include <esp_sleep.h>
#include "config.h"

// DCF77 signal pin
#define DCF77_PIN 4  // D2 on XIAO ESP32C3

// Time sync parameters
#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SEC 3600      // CET = UTC+1
#define DAYLIGHT_OFFSET_SEC 3600 // CEST = UTC+2 in summer

// Casio multiband sync hours (when watches typically sync)
// Most Casio watches sync at 2 AM, 3 AM, 4 AM, or 5 AM local time
const int SYNC_HOURS[] = {2, 3, 4, 5};
const int NUM_SYNC_HOURS = 4;

// DCF77 timing
#define DCF77_BIT_LENGTH 1000    // 1 second per bit
#define DCF77_CARRIER_LOW 100    // 100ms for logic 0
#define DCF77_CARRIER_HIGH 200   // 200ms for logic 1
#define DCF77_MINUTE_MARK 0      // No carrier for minute marker

// Global variables
RTC_DATA_ATTR int bootCount = 0;
bool timeInitialized = false;
struct tm currentTime;

// Function declarations
void setupWiFi();
void syncNTPTime();
void transmitDCF77Signal();
void encodeDCF77Minute(uint8_t dcf77Buffer[]);
uint8_t calculateParity(uint8_t bits[], int start, int end);
void setupDeepSleep();
bool isNextSyncHour(int currentHour);
int getSecondsUntilNextSync();

void setup() {
  Serial.begin(115200);
  delay(100);
  
  ++bootCount;
  Serial.println("\n=== DCF77 Emulator for XIAO ESP32C3 ===");
  Serial.printf("Boot count: %d\n", bootCount);
  
  // Configure DCF77 output pin
  pinMode(DCF77_PIN, OUTPUT);
  digitalWrite(DCF77_PIN, LOW);
  
  // Connect to WiFi
  setupWiFi();
  
  // Sync time with NTP
  syncNTPTime();
  
  // Wait for time to be properly set
  int retries = 0;
  while (!getLocalTime(&currentTime) && retries < 10) {
    Serial.println("Waiting for time to be set...");
    delay(500);
    retries++;
  }
  
  if (retries >= 10) {
    Serial.println("Failed to obtain time. Restarting...");
    ESP.restart();
  }
  
  timeInitialized = true;
  Serial.printf("Current time: %02d:%02d:%02d %02d.%02d.%04d\n",
                currentTime.tm_hour, currentTime.tm_min, currentTime.tm_sec,
                currentTime.tm_mday, currentTime.tm_mon + 1, currentTime.tm_year + 1900);
  
  // Disconnect WiFi to save power
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  Serial.println("WiFi disconnected to save power");
}

void loop() {
  if (timeInitialized) {
    getLocalTime(&currentTime);
    
    // Check if we're at the start of a new minute
    if (currentTime.tm_sec == 0) {
      Serial.printf("\n=== Transmitting DCF77 signal for %02d:%02d ===\n",
                    currentTime.tm_hour, currentTime.tm_min);
      transmitDCF77Signal();
      
      // After transmitting, check if we should sleep
      getLocalTime(&currentTime);
      
      // If we've been transmitting for a while (e.g., 1 hour) and next sync is far away
      if (currentTime.tm_min >= 0) {  // Adjust based on desired transmission duration
        int secondsToNextSync = getSecondsUntilNextSync();
        
        // If next sync is more than 10 minutes away, consider going to sleep
        if (secondsToNextSync > 600) {
          Serial.printf("Next sync in %d seconds. Going to deep sleep...\n", secondsToNextSync);
          setupDeepSleep();
          esp_deep_sleep_start();
        }
      }
    }
    
    delay(1000);
  } else {
    delay(1000);
  }
}

void setupWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to WiFi. Restarting...");
    ESP.restart();
  }
}

void syncNTPTime() {
  Serial.println("Synchronizing time with NTP server...");
  
  configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER, "time.nist.gov");
  
  // Wait for time to be set
  int retry = 0;
  const int retry_count = 10;
  while (time(nullptr) < 100000 && retry < retry_count) {
    Serial.print(".");
    delay(1000);
    retry++;
  }
  
  if (retry < retry_count) {
    Serial.println("\nNTP time synchronized successfully!");
  } else {
    Serial.println("\nFailed to synchronize NTP time!");
  }
}

void transmitDCF77Signal() {
  uint8_t dcf77Buffer[60] = {0};
  
  // Get current time
  getLocalTime(&currentTime);
  
  // Encode the NEXT minute (DCF77 transmits the time of the upcoming minute)
  struct tm nextMinute = currentTime;
  nextMinute.tm_sec = 0;
  nextMinute.tm_min++;
  mktime(&nextMinute);  // Normalize the time structure
  
  encodeDCF77Minute(dcf77Buffer);
  
  // Transmit 60 bits (one per second)
  for (int second = 0; second < 60; second++) {
    unsigned long bitStart = millis();
    
    if (second == 0) {
      // Minute marker: no carrier for entire second
      digitalWrite(DCF77_PIN, LOW);
      Serial.print("M");
    } else {
      // Bit transmission
      int carrierDuration = dcf77Buffer[second] ? DCF77_CARRIER_HIGH : DCF77_CARRIER_LOW;
      
      // Carrier on (reduced amplitude)
      digitalWrite(DCF77_PIN, LOW);
      delay(carrierDuration);
      
      // Carrier off (full amplitude)
      digitalWrite(DCF77_PIN, HIGH);
      
      Serial.print(dcf77Buffer[second]);
    }
    
    // Wait for the rest of the second
    unsigned long elapsed = millis() - bitStart;
    if (elapsed < DCF77_BIT_LENGTH) {
      delay(DCF77_BIT_LENGTH - elapsed);
    }
    
    if ((second + 1) % 10 == 0) Serial.print(" ");
  }
  
  Serial.println();
}

void encodeDCF77Minute(uint8_t dcf77Buffer[]) {
  // Get the time for the NEXT minute
  struct tm nextMinute = currentTime;
  nextMinute.tm_min++;
  mktime(&nextMinute);
  
  // Clear buffer
  memset(dcf77Buffer, 0, 60);
  
  // Bit 0: Minute marker (always 0, transmitted as no carrier)
  dcf77Buffer[0] = 0;
  
  // Bits 1-14: Reserved/civil warning bits (normally 0)
  // Bit 15: Call bit (0)
  // Bit 16: Summer time announcement (0 or 1)
  dcf77Buffer[16] = (nextMinute.tm_isdst > 0) ? 0 : 1;  // Inverted: 1 = CET, 0 = CEST
  
  // Bit 17: CEST (summer time)
  dcf77Buffer[17] = (nextMinute.tm_isdst > 0) ? 1 : 0;
  
  // Bit 18: CET (standard time)
  dcf77Buffer[18] = (nextMinute.tm_isdst > 0) ? 0 : 1;
  
  // Bit 19: Leap second announcement (0)
  dcf77Buffer[19] = 0;
  
  // Bit 20: Start of time code (always 1)
  dcf77Buffer[20] = 1;
  
  // Bits 21-27: Minute (BCD)
  int minute = nextMinute.tm_min;
  dcf77Buffer[21] = (minute & 0x01) ? 1 : 0;
  dcf77Buffer[22] = (minute & 0x02) ? 1 : 0;
  dcf77Buffer[23] = (minute & 0x04) ? 1 : 0;
  dcf77Buffer[24] = (minute & 0x08) ? 1 : 0;
  dcf77Buffer[25] = (minute & 0x10) ? 1 : 0;
  dcf77Buffer[26] = (minute & 0x20) ? 1 : 0;
  dcf77Buffer[27] = (minute & 0x40) ? 1 : 0;
  
  // Bit 28: Minute parity
  dcf77Buffer[28] = calculateParity(dcf77Buffer, 21, 27);
  
  // Bits 29-34: Hour (BCD)
  int hour = nextMinute.tm_hour;
  dcf77Buffer[29] = (hour & 0x01) ? 1 : 0;
  dcf77Buffer[30] = (hour & 0x02) ? 1 : 0;
  dcf77Buffer[31] = (hour & 0x04) ? 1 : 0;
  dcf77Buffer[32] = (hour & 0x08) ? 1 : 0;
  dcf77Buffer[33] = (hour & 0x10) ? 1 : 0;
  dcf77Buffer[34] = (hour & 0x20) ? 1 : 0;
  
  // Bit 35: Hour parity
  dcf77Buffer[35] = calculateParity(dcf77Buffer, 29, 34);
  
  // Bits 36-41: Day of month (BCD)
  int day = nextMinute.tm_mday;
  dcf77Buffer[36] = (day & 0x01) ? 1 : 0;
  dcf77Buffer[37] = (day & 0x02) ? 1 : 0;
  dcf77Buffer[38] = (day & 0x04) ? 1 : 0;
  dcf77Buffer[39] = (day & 0x08) ? 1 : 0;
  dcf77Buffer[40] = (day & 0x10) ? 1 : 0;
  dcf77Buffer[41] = (day & 0x20) ? 1 : 0;
  
  // Bits 42-44: Day of week (1=Monday, 7=Sunday)
  int dayOfWeek = (nextMinute.tm_wday == 0) ? 7 : nextMinute.tm_wday;
  dcf77Buffer[42] = (dayOfWeek & 0x01) ? 1 : 0;
  dcf77Buffer[43] = (dayOfWeek & 0x02) ? 1 : 0;
  dcf77Buffer[44] = (dayOfWeek & 0x04) ? 1 : 0;
  
  // Bits 45-49: Month (BCD)
  int month = nextMinute.tm_mon + 1;
  dcf77Buffer[45] = (month & 0x01) ? 1 : 0;
  dcf77Buffer[46] = (month & 0x02) ? 1 : 0;
  dcf77Buffer[47] = (month & 0x04) ? 1 : 0;
  dcf77Buffer[48] = (month & 0x08) ? 1 : 0;
  dcf77Buffer[49] = (month & 0x10) ? 1 : 0;
  
  // Bits 50-57: Year (BCD, last 2 digits)
  int year = (nextMinute.tm_year + 1900) % 100;
  dcf77Buffer[50] = (year & 0x01) ? 1 : 0;
  dcf77Buffer[51] = (year & 0x02) ? 1 : 0;
  dcf77Buffer[52] = (year & 0x04) ? 1 : 0;
  dcf77Buffer[53] = (year & 0x08) ? 1 : 0;
  dcf77Buffer[54] = (year & 0x10) ? 1 : 0;
  dcf77Buffer[55] = (year & 0x20) ? 1 : 0;
  dcf77Buffer[56] = (year & 0x40) ? 1 : 0;
  dcf77Buffer[57] = (year & 0x80) ? 1 : 0;
  
  // Bit 58: Date parity
  dcf77Buffer[58] = calculateParity(dcf77Buffer, 36, 57);
  
  // Bit 59: No bit (minute marker preparation)
  dcf77Buffer[59] = 0;
  
  Serial.printf("Encoded time: %02d:%02d %02d.%02d.%04d (Day %d)\n",
                nextMinute.tm_hour, nextMinute.tm_min,
                nextMinute.tm_mday, nextMinute.tm_mon + 1, nextMinute.tm_year + 1900,
                dayOfWeek);
}

uint8_t calculateParity(uint8_t bits[], int start, int end) {
  int count = 0;
  for (int i = start; i <= end; i++) {
    if (bits[i]) count++;
  }
  return (count % 2 == 0) ? 0 : 1;  // Even parity
}

void setupDeepSleep() {
  int secondsToSleep = getSecondsUntilNextSync();
  
  Serial.printf("Setting deep sleep for %d seconds\n", secondsToSleep);
  
  // Configure wake up timer
  esp_sleep_enable_timer_wakeup(secondsToSleep * 1000000ULL);
}

bool isNextSyncHour(int currentHour) {
  for (int i = 0; i < NUM_SYNC_HOURS; i++) {
    if (currentHour == SYNC_HOURS[i]) {
      return true;
    }
  }
  return false;
}

int getSecondsUntilNextSync() {
  struct tm now;
  getLocalTime(&now);
  
  int currentHour = now.tm_hour;
  int currentMin = now.tm_min;
  int currentSec = now.tm_sec;
  
  // Find next sync hour
  int nextSyncHour = -1;
  for (int i = 0; i < NUM_SYNC_HOURS; i++) {
    if (SYNC_HOURS[i] > currentHour) {
      nextSyncHour = SYNC_HOURS[i];
      break;
    }
  }
  
  // If no sync hour found today, use first sync hour tomorrow
  if (nextSyncHour == -1) {
    nextSyncHour = SYNC_HOURS[0] + 24;
  }
  
  // Calculate seconds until next sync
  int hoursUntilSync = nextSyncHour - currentHour;
  int secondsUntilSync = (hoursUntilSync * 3600) - (currentMin * 60) - currentSec;
  
  // Wake up 5 minutes before the sync hour to be ready
  secondsUntilSync -= 300;
  
  if (secondsUntilSync < 60) {
    secondsUntilSync = 60;  // Minimum 1 minute
  }
  
  return secondsUntilSync;
}
