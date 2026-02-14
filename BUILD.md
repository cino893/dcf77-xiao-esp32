# Build and Upload Guide

## 🔧 Environment Setup

### Step 1: Install Arduino IDE

#### Windows:
1. Download from https://www.arduino.cc/en/software
2. Select "Windows Win 10 and newer"
3. Install (recommended: Install for All Users)

#### macOS:
1. Download from https://www.arduino.cc/en/software
2. Select "macOS"
3. Drag to Applications folder

#### Linux:
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install arduino

# Or download the latest version:
wget https://downloads.arduino.cc/arduino-ide/arduino-ide_latest_Linux_64bit.AppImage
chmod +x arduino-ide_*_Linux_64bit.AppImage
./arduino-ide_*_Linux_64bit.AppImage
```

### Step 2: Add ESP32 Support

1. **Open Arduino IDE**

2. **Add URL to Board Manager**:
   - **Arduino IDE 2.x**: File → Preferences → Additional boards manager URLs
   - **Arduino IDE 1.x**: File → Preferences → Additional Boards Manager URLs
   
   Add:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```

3. **Install ESP32 Board Package**:
   - Tools → Board → Boards Manager
   - Search: "esp32"
   - Find "esp32 by Espressif Systems"
   - Click "Install" (may take a few minutes)

### Step 3: Configure XIAO ESP32C3

1. **Connect XIAO ESP32C3 to USB**
   - Use a good USB-C cable (with data transmission)
   - Wait for device detection

2. **Select board**:
   - Tools → Board → ESP32 Arduino → **XIAO_ESP32C3**

3. **Select port**:
   - Tools → Port → Select appropriate port
   - **Windows**: COM3, COM4, etc.
   - **macOS**: /dev/cu.usbmodem*
   - **Linux**: /dev/ttyACM0, /dev/ttyUSB0

4. **Configure additional options**:
   - Tools → Upload Speed → **921600** (fastest)
   - Tools → USB CDC On Boot → **Enabled** (for Serial Monitor)
   - Tools → CPU Frequency → **160MHz** (default)
   - Tools → Flash Size → **4MB** (default)
   - Tools → Partition Scheme → **Default 4MB**

## 📥 Downloading the Project

### Option A: Git Clone (recommended)
```bash
git clone https://github.com/cino893/dcf77-xiao-esp32.git
cd dcf77-xiao-esp32
```

### Option B: Download ZIP
1. Go to https://github.com/cino893/dcf77-xiao-esp32
2. Click "Code" → "Download ZIP"
3. Extract to directory `dcf77-xiao-esp32`

## ⚙️ Configuration

### Step 1: Copy config.h
```bash
# In project directory:
cp config.h.example config.h
```

### Step 2: Edit config.h
Open `config.h` in a text editor and set your WiFi credentials:

```cpp
#define WIFI_SSID "YourSSID"           // WiFi network name (2.4GHz!)
#define WIFI_PASSWORD "YourPassword"   // WiFi password
```

**⚠️ IMPORTANT:**
- WiFi MUST be 2.4GHz (ESP32C3 does not support 5GHz)
- SSID and password are case-sensitive
- Do not commit config.h with real credentials to GitHub

## 🔨 Compile and Upload

### Method 1: Arduino IDE (GUI)

1. **Open project**:
   - File → Open
   - Select `dcf77-xiao-esp32.ino`

2. **Verify code** (optional):
   - Sketch → Verify/Compile (Ctrl+R / Cmd+R)
   - Wait for "Done compiling" message

3. **Upload code**:
   - Sketch → Upload (Ctrl+U / Cmd+U)
   - Observe progress at bottom of window
   
   **If connection error occurs:**
   1. Hold **BOOT** button on XIAO
   2. Click Upload
   3. Release BOOT when upload starts

4. **Open Serial Monitor**:
   - Tools → Serial Monitor (Ctrl+Shift+M)
   - Set baud rate: **115200**
   - You should see startup logs

### Method 2: Arduino CLI (advanced)

#### Install Arduino CLI:
```bash
# Linux/macOS
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh

# Windows (PowerShell)
iwr -useb https://raw.githubusercontent.com/arduino/arduino-cli/master/install.ps1 | iex
```

#### Configuration:
```bash
# Initialize
arduino-cli config init

# Add ESP32 URL
arduino-cli config add board_manager.additional_urls https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

# Update index
arduino-cli core update-index

# Install ESP32
arduino-cli core install esp32:esp32
```

#### Compile:
```bash
cd dcf77-xiao-esp32
arduino-cli compile --fqbn esp32:esp32:XIAO_ESP32C3 dcf77-xiao-esp32.ino
```

#### Upload:
```bash
# Check port
arduino-cli board list

# Upload (replace PORT with appropriate value)
arduino-cli upload -p /dev/ttyACM0 --fqbn esp32:esp32:XIAO_ESP32C3 dcf77-xiao-esp32.ino
```

#### Serial monitor:
```bash
arduino-cli monitor -p /dev/ttyACM0 -c baudrate=115200
```

## 🧪 Testing

### Test 1: Basic GPIO Test
```bash
# Open and upload:
examples/simple_gpio_test/simple_gpio_test.ino
```
**Expected result**: LED blinks once per second

### Test 2: WiFi and NTP Test
```bash
# Edit WiFi credentials in the file
# Then open and upload:
examples/wifi_ntp_test/wifi_ntp_test.ino
```
**Expected result**: WiFi connection and time display

### Test 3: Full DCF77
```bash
# Upload main sketch
dcf77-xiao-esp32.ino
```
**Expected result**: See "Expected Logs" below

## 📊 Expected Logs

### Successful startup:
```
=== DCF77 Emulator for XIAO ESP32C3 ===
Boot count: 1
Connecting to WiFi: YourSSID
..........
WiFi connected!
IP address: 192.168.1.100
Synchronizing time with NTP server...
..
NTP time synchronized successfully!
Current time: 14:23:45 13.02.2026
WiFi disconnected to save power

=== Transmitting DCF77 signal for 14:24 ===
Encoded time: 14:25 13.02.2026 (Day 4)
M0100000100 0000011000 1000100001 0001100100 0001000101 0100100
```

### Errors and solutions:

#### "Failed to connect to WiFi"
```
Connecting to WiFi: YourSSID
..............................
Failed to connect to WiFi. Restarting...
```
**Solution**: Check SSID, password and that WiFi is 2.4GHz

#### "Failed to synchronize NTP time"
```
Synchronizing time with NTP server...
..........
Failed to synchronize NTP time!
```
**Solution**: Check firewall, router, try different NTP server

## 🐛 Build Troubleshooting

### Error: "Board not found"
```
Error: Board esp32:esp32:XIAO_ESP32C3 is unknown
```
**Solution**: 
1. Check if you installed ESP32 board package
2. Restart Arduino IDE
3. Try reinstalling ESP32

### Error: "Port not found"
```
Error: port not found
```
**Solution**:
1. Check if USB cable transmits data (not just power)
2. Install USB-Serial drivers (Windows)
3. Check Device Manager (Windows) or dmesg (Linux)

### Error: "Timeout waiting for packet"
```
A fatal error occurred: Failed to connect to ESP32
```
**Solution**:
1. **Method 1**: Hold BOOT during upload
2. **Method 2**: Lower Upload Speed (to 115200)
3. **Method 3**: Press RESET before upload

### Error: "Not enough space"
```
Sketch uses 1450000 bytes (110%) of program storage
```
**Solution**: Should not happen with this project. Check if:
1. Selected correct board (XIAO_ESP32C3)
2. Flash Size set to 4MB

### Error: "config.h: No such file"
```
dcf77-xiao-esp32.ino:15:10: fatal error: config.h: No such file or directory
```
**Solution**:
```bash
cp config.h.example config.h
# Then edit config.h
```

## 📝 Pre-Upload Checklist

- [ ] Arduino IDE installed with ESP32 support
- [ ] XIAO ESP32C3 connected via USB
- [ ] Correct board selected (XIAO_ESP32C3)
- [ ] Correct port selected
- [ ] config.h exists and contains valid WiFi credentials
- [ ] WiFi is 2.4GHz (not 5GHz)
- [ ] Serial Monitor set to 115200 baud
- [ ] USB CDC On Boot enabled

## 🔄 Updates

### Update code from Git:
```bash
cd dcf77-xiao-esp32
git pull origin main
```

### Update ESP32 Core:
1. Tools → Board → Boards Manager
2. Find "esp32"
3. Click "Update" if available

## 💾 Configuration Backup

**IMPORTANT**: Before updating, backup your `config.h`:
```bash
cp config.h config.h.backup
```

After update:
```bash
# If config.h was overwritten:
cp config.h.backup config.h
```

## 🎯 Next Steps

After successful upload:
1. Go to [HARDWARE.md](HARDWARE.md) - circuit assembly
2. Go to [README.md](README.md) - usage
3. In case of problems: [TROUBLESHOOTING.md](TROUBLESHOOTING.md)

---

**Good luck with uploading!** 🚀
