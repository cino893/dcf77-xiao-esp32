# DCF77 Emulator for Xiao ESP32 C3
### Project Description

DCF77 signal emulator for Casio Multiband watches and other radio-controlled clocks. Device based on **Seeed Studio XIAO ESP32C3** synchronizes time via WiFi (NTP), then emits DCF77 signal that can be received by watches within 5-15 meters range.

### ✨ Key Features

- 🕐 **Automatic synchronization**: Wakes up during Casio watch sync hours (2-5 AM)
- 📡 **DCF77 emulation**: Full DCF77 protocol implementation with BCD encoding
- 🎚️ **PWM amplitude modulation**: Precise signal levels for Casio compatibility (~20% AM)
- 🌐 **NTP synchronization**: Fetches accurate time from NTP servers via WiFi
- 🔋 **Power saving**: Deep sleep between transmissions (~40-50μA)
- 💰 **Cheap components**: Total cost ~5-8 USD (AliExpress parts)
- 📏 **5-15m range**: Sufficient for home/apartment

### 🛠️ Required Components

1. **Seeed Studio XIAO ESP32C3** (~$3.50)
2. **NPN Transistor** (BC547 or 2N2222) (~$0.02)
3. **Resistors**: 1kΩ and 10kΩ (~$0.02)
4. **Capacitor 100nF** (~$0.02)
5. **Ferrite rod antenna** with wound wire (~$1-5)
   - Ferrite rod 10mm × 100mm
   - ~200 turns of 0.3mm enameled wire

**Total cost**: ~$5-8 USD

Detailed schematic and assembly instructions: see [HARDWARE.md](HARDWARE.md)

### 📦 Software Installation

#### Prerequisites

1. **Arduino IDE** (version 1.8.x or 2.x)
2. **ESP32 support** in Arduino IDE:
   - File → Preferences → Additional Board Manager URLs
   - Add: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
   - Tools → Board → Boards Manager → Search "ESP32" → Install

#### Configuration

1. **Clone repository**:
```bash
git clone https://github.com/cino893/dcf77-xiao-esp32.git
cd dcf77-xiao-esp32
```

2. **Configure WiFi**:
```bash
cp config.h.example config.h
```

3. **Edit `config.h`** and set your WiFi credentials:
```cpp
#define WIFI_SSID "YourSSID"
#define WIFI_PASSWORD "YourPassword"

// For Casio watches: recommended PWM settings (default)
#define DCF77_PWM_MODE true          // PWM mode for Casio
#define DCF77_AMPLITUDE_LOW 51       // 20% amplitude (LOW)
#define DCF77_AMPLITUDE_HIGH 0       // 0% amplitude (HIGH)
#define DCF77_PWM_FREQUENCY 2000     // 2 kHz PWM
```

> ⚠️ **Important for Casio watches**: PWM mode with ~20% amplitude is **required** for proper synchronization of most Casio watches. See [DCF77_SIGNAL_LEVELS.md](DCF77_SIGNAL_LEVELS.md) for details.

4. **Open project in Arduino IDE**:
   - Open file `dcf77-xiao-esp32.ino`

5. **Select board**:
   - Tools → Board → ESP32 Arduino → XIAO_ESP32C3

6. **Configure options**:
   - Tools → Upload Speed → 921600
   - Tools → USB CDC On Boot → Enabled

7. **Upload code**:
   - Sketch → Upload

### 🔌 Hardware Connection

```
XIAO ESP32C3 Pin D2 (GPIO4) → 1kΩ Resistor → BC547 Transistor Base
Transistor Emitter → GND
Transistor Collector → Antenna (coil start)
Antenna (coil end) → GND
10kΩ between base and GND (optional)
```

Full schematic: see [HARDWARE.md](HARDWARE.md)

### 🚀 Usage

1. **Connect power** (USB or 3.7V Li-Po battery)
2. **Device will automatically**:
   - Connect to WiFi
   - Synchronize time with NTP
   - Start DCF77 signal transmission
3. **Set Casio watch** to manual sync mode:
   - Place watch 10-50cm from antenna
   - Turn off other interference sources (phones, computers)
   - Start synchronization on watch (see watch manual)
4. **Monitor serial output** (115200 baud) - you'll see transmitted bits

### ⏰ Operation Schedule

Device automatically wakes during Casio sync hours:
- **2:00** - 3:00 AM
- **3:00** - 4:00 AM
- **4:00** - 5:00 AM
- **5:00** - 6:00 AM

Between sessions it enters deep sleep (~40-50μA).

### 🔧 Troubleshooting

**Watch won't synchronize:**
- Check connection quality
- Ensure antenna is properly wound
- Reduce distance (10-30cm)
- Check antenna orientation (rotate 90°)
- Verify time in serial monitor

**WiFi won't connect:**
- Check SSID and password in `config.h`
- Ensure WiFi network is 2.4GHz (not 5GHz)
- Check WiFi range

**Weak range:**
- Increase antenna turns (up to 250)
- Use longer ferrite rod
- Add MOSFET amplifier (see HARDWARE.md)

### 📊 Technical Specifications

- **Microcontroller**: ESP32-C3 (RISC-V, 160MHz)
- **Protocol**: DCF77 (amplitude modulation)
- **Frequency**: 77.5 kHz (emulated)
- **Data format**: BCD (Binary Coded Decimal)
- **Time sync**: NTP (Network Time Protocol)
- **Timezone**: CET/CEST (UTC+1/+2)
- **Power consumption**: 40-50μA (sleep) / 80-120mA (transmit)
- **Range**: 5-15 meters (antenna dependent)

### 📝 License

MIT License - see [LICENSE](LICENSE)

### 🤝 Contributing

Pull requests are welcome! For major changes, please open an issue first.

### 📚 References

**Project Documentation:**
- [DCF77_SIGNAL_LEVELS.md](DCF77_SIGNAL_LEVELS.md) - DCF77 signal levels and Casio requirements
- [TESTING.md](TESTING.md) - Comprehensive testing guide
- [TROUBLESHOOTING.md](TROUBLESHOOTING.md) - Troubleshooting and problem solving
- [HARDWARE.md](HARDWARE.md) - Circuit diagram and hardware assembly
- [ADVANCED.md](ADVANCED.md) - Advanced modifications
- [IMPLEMENTATION_SUMMARY.md](IMPLEMENTATION_SUMMARY.md) - Implementation summary v2.0

**External Resources:**
- [DCF77 Protocol Specification](https://en.wikipedia.org/wiki/DCF77)
- [XIAO ESP32C3 Documentation](https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/)
- [ESP32 Deep Sleep Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-reference/system/sleep_modes.html)