# Troubleshooting Guide

## 🔍 Problem Diagnosis

### 1. ❌ Device won't start

**Symptoms:**
- No output on serial monitor
- LED not blinking
- No response

**Solutions:**

#### A. Check power supply
```bash
# You should see in serial monitor:
=== DCF77 Emulator for XIAO ESP32C3 ===
Boot count: 1
```

**If you don't see the text:**
- ✅ Check USB cable (try another one)
- ✅ Check computer USB port
- ✅ Ensure you selected the correct port in Arduino IDE
- ✅ Increase delay after opening serial monitor (2-3 seconds)

#### B. Upload problem
```bash
# Error: "Failed to connect to ESP32"
```

**Solution:**
1. Hold BOOT button on XIAO
2. Click Upload in Arduino IDE
3. Wait until upload starts
4. Release BOOT button

---

### 2. 📡 WiFi won't connect

**Symptoms:**
```bash
Connecting to WiFi: YourSSID
............................
Failed to connect to WiFi. Restarting...
```

**Solutions:**

#### A. Check credentials in config.h
```cpp
// config.h - MUST match EXACTLY
#define WIFI_SSID "YourNetworkName"      // Case sensitive!
#define WIFI_PASSWORD "YourPassword123"   // Case sensitive!
```

**Common mistakes:**
- ❌ Leading/trailing spaces in SSID or password
- ❌ Case sensitivity (case sensitive)
- ❌ Hidden characters (copy-paste may add)
- ❌ Incorrect password

#### B. Check network type
**XIAO ESP32C3 supports ONLY 2.4GHz!**

- ❌ Doesn't work with: 5GHz, WiFi 6E
- ✅ Works with: 2.4GHz (802.11 b/g/n)

**How to check:**
1. On phone: Settings → WiFi → Network info
2. On router: Admin panel
3. Try using phone as hotspot (2.4GHz)

#### C. Range issues
```cpp
// In setup(), add logs:
Serial.print("WiFi RSSI: ");
Serial.println(WiFi.RSSI());
// RSSI > -70 dBm = good signal
// RSSI < -80 dBm = weak, move closer to router
```

---

### 3. ⏰ Time won't sync

**Symptoms:**
```bash
Synchronizing time with NTP server...
..........
Failed to synchronize NTP time!
```

**Solutions:**

#### A. Check firewall
NTP uses UDP port 123. Make sure that:
- ✅ Router doesn't block NTP
- ✅ Firewall doesn't block ESP32

#### B. Change NTP server
```cpp
// In dcf77-xiao-esp32.ino, change:
#define NTP_SERVER "time.google.com"  // Google NTP
// or
#define NTP_SERVER "time.cloudflare.com"  // Cloudflare
// or local
#define NTP_SERVER "192.168.1.1"  // Your router
```

#### C. Check timezone
```cpp
// Make sure timezone is correct:
#define GMT_OFFSET_SEC 3600        // CET (UTC+1)
#define DAYLIGHT_OFFSET_SEC 3600   // +1h in summer

// For Poland: OK ✅
// For UK: 0, 3600
// For USA EST: -18000, 3600
```

---

### 4. 📻 Watch won't sync

Most common problem!

**Symptoms:**
- ESP32 works, transmitting signal
- Watch doesn't detect signal
- Synchronization fails

**Step-by-step solutions:**

#### Step 1: Check transmission
You should see in serial monitor:
```bash
=== Transmitting DCF77 signal for 03:25 ===
Encoded time: 03:26 13.02.2026 (Day 5)
M0100000100 0000011000 1000100001...
```

**If no transmission:**
- Time might be incorrect
- Check minute (must be :00 seconds)

#### Step 2: LED Test
Connect LED + 220Ω resistor to GPIO4:
```
GPIO4 ─────[220Ω]─────(LED)─────GND
```

**LED should blink once per second**
- If not blinking: Problem with code or GPIO
- If blinking: Hardware works ✅

#### Step 3: Check antenna

##### A. Polarity
```bash
# Check connection:
Q1 Collector → Antenna coil start
Coil end → GND
```

**Reversed connection = no signal!**

##### B. Number of turns
- Minimum: 150 turns
- Optimal: 200 turns  
- Maximum: 300 turns

**If too few turns: Range < 1m**

##### C. Winding quality
- ❌ Loose, uneven turns
- ✅ Tight, even turns, center of rod

#### Step 4: Watch positioning

**VERY IMPORTANT!**

```
        [Ferrite antenna - vertical]
              |
              |  10-30 cm
              ↓
        [Watch - flat]
        
GOOD orientation ✅:
- Watch FLAT on table
- Antenna VERTICAL or HORIZONTAL (test both)
- Distance 10-30 cm

BAD orientation ❌:
- Watch at angle
- Too far (>50cm)
- Too close (<5cm - saturation)
```

**Orientation test:**
1. Place watch flat
2. Hold antenna vertically above watch (~20cm)
3. Slowly rotate watch 90° (4 positions)
4. Wait 15-30 seconds in each position
5. One position should work

#### Step 5: Transmission timing

Casio synchronizes ONLY at specific hours:
- 🕑 2:00 - 3:00
- 🕒 3:00 - 4:00
- 🕓 4:00 - 5:00
- 🕔 5:00 - 6:00

**Outside these hours:**
- Some watches REQUIRE manual synchronization
- Press and hold ADJUST button ~2 seconds
- Watch should display "RCV" or similar

#### Step 6: Signal amplification

**If still not working, increase power:**

##### Option A: More turns
```bash
Increase to 250-300 turns
Use longer rod (150-200mm)
```

##### Option B: MOSFET amplifier
```
Replace BC547 with IRF540N:
- Gate → GPIO4 (through 1kΩ)
- Source → GND
- Drain → Antenna
```

##### Option C: Better wire
```bash
Instead of 0.3mm use 0.4-0.5mm
Lower resistance = higher current = stronger field
```

#### Step 7: Signal Levels (IMPORTANT for Casio!)

**Problem: Casio needs ~20% amplitude for LOW signal**

⚠️ **Most common cause of Casio sync failure!**

Casio uses a special detector that requires:
- LOW amplitude (carrier on): ~15-25% power
- HIGH amplitude (carrier off): 0% power

**Solution: Use PWM mode**

##### Step 7.1: Check configuration
In `config.h` file:
```cpp
// MUST be enabled for Casio:
#define DCF77_PWM_MODE true

// LOW amplitude: 20% (recommended for Casio)
#define DCF77_AMPLITUDE_LOW 51    // 51/255 ≈ 20%

// HIGH amplitude: 0% (no signal)
#define DCF77_AMPLITUDE_HIGH 0

// PWM frequency: 2 kHz
#define DCF77_PWM_FREQUENCY 2000
```

##### Step 7.2: Verify in serial monitor
After startup you should see:
```bash
Configuring DCF77 output...
  Mode: PWM amplitude modulation
  LOW amplitude: 51 (~20.0%)
  HIGH amplitude: 0 (~0.0%)
  PWM frequency: 2000 Hz
```

**If you see "Simple GPIO on/off (legacy)":**
- ❌ PWM mode is NOT active
- ✅ Change `DCF77_PWM_MODE true` in config.h
- ✅ Upload code again

##### Step 7.3: Fine-tune amplitude
If watch still won't sync, test different values:

**For most Casio (recommended):**
```cpp
#define DCF77_AMPLITUDE_LOW 51    // 20%
```

**For difficult models (try in order):**
```cpp
#define DCF77_AMPLITUDE_LOW 64    // 25% - try first
#define DCF77_AMPLITUDE_LOW 45    // 18%
#define DCF77_AMPLITUDE_LOW 38    // 15%
#define DCF77_AMPLITUDE_LOW 58    // 23%
```

**After each change:**
1. Upload code again
2. Check serial monitor (should show new value)
3. Test for 3-5 minutes
4. If doesn't work, try next value

##### Step 7.4: Test with oscilloscope (optional)
If you have an oscilloscope, measure on GPIO4:
- During LOW pulse: 2kHz PWM with ~20% duty cycle
- During HIGH: Constant 0V
- LOW pulse time: 100ms (bit 0) or 200ms (bit 1)

**See details:** [DCF77_SIGNAL_LEVELS.md](DCF77_SIGNAL_LEVELS.md)

---

### 5. 🔋 Battery drains quickly

**Expected consumption:**
- Deep sleep: 40-50 μA
- Transmission: 80-120 mA
- WiFi active: 100-180 mA

**If battery drains in <24h:**

#### A. Deep sleep not working
```cpp
// Add logs before sleep:
Serial.println("Going to sleep...");
Serial.flush();  // Important! Ensure data is sent
delay(100);
esp_deep_sleep_start();
```

#### B. WiFi not turning off
```cpp
// After NTP sync, check:
WiFi.disconnect(true);
WiFi.mode(WIFI_OFF);
delay(100);
```

#### C. Too frequent transmission
```cpp
// Change schedule (only 2:00-3:00):
const int SYNC_HOURS[] = {2};
const int NUM_SYNC_HOURS = 1;
```

---

### 6. 🐛 Other issues

#### A. Reset loop (continuous restarting)
```bash
=== DCF77 Emulator ===
Boot count: 15
Boot count: 16
Boot count: 17
...
```

**Causes:**
- Watchdog timeout (code takes too long)
- Memory problems
- Corrupted code

**Solution:**
```bash
# Erase flash memory:
python -m esptool --port /dev/ttyUSB0 erase_flash

# Upload code again
```

#### B. Serial monitor shows garbage
```bash
ÿþý����������
```

**Solution:**
1. Set baud rate: **115200**
2. Check USB CDC On Boot: **Enabled**
3. Press reset on XIAO
4. Open serial monitor IMMEDIATELY after reset

---

## 🧪 Diagnostic Tests

### Test 1: Basic Test
```cpp
// Simple GPIO test:
void setup() {
  pinMode(4, OUTPUT);
}

void loop() {
  digitalWrite(4, HIGH);
  delay(500);
  digitalWrite(4, LOW);
  delay(500);
}
// LED should blink at 1Hz
```

### Test 2: WiFi Test
```cpp
void setup() {
  Serial.begin(115200);
  WiFi.begin("SSID", "PASSWORD");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.println(WiFi.localIP());
}
```

### Test 3: NTP Test
```cpp
void setup() {
  // ... WiFi setup ...
  configTime(3600, 3600, "pool.ntp.org");
  
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  }
}
```

---

## 📞 Further Support

If none of the above steps helped:

1. **GitHub Issues**: https://github.com/cino893/dcf77-xiao-esp32/issues
2. **Include**:
   - Logs from serial monitor (full output)
   - Casio watch model
   - Photo of antenna and connections
   - `config.h` code (WITHOUT WiFi password!)
3. **Describe in detail**:
   - What you tried
   - When the problem occurs
   - What steps you followed

---

**Good luck!** 🍀
