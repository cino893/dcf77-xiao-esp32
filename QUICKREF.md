# Quick Reference

## ⚡ Quick Start

### 5-minute setup:

1. **Clone repo**:
   ```bash
   git clone https://github.com/cino893/dcf77-xiao-esp32.git
   ```

2. **Configure WiFi**:
   ```bash
   cd dcf77-xiao-esp32
   cp config.h.example config.h
   nano config.h  # Edit SSID and password
   ```

3. **Upload code**:
   - Open `dcf77-xiao-esp32.ino` in Arduino IDE
   - Tools → Board → XIAO_ESP32C3
   - Tools → Port → [select port]
   - Upload (Ctrl+U)

4. **Build hardware**:
   - See schematic in [HARDWARE.md](HARDWARE.md)
   - Connect: GPIO4 → 1kΩ → BC547 (base) → antenna

5. **Test**:
   - Place Casio watch ~20cm from antenna
   - Set to manual sync mode
   - Wait ~1 minute

## 📊 Pin Layout

```
XIAO ESP32C3 Pins:
┌─────────────────┐
│  [USB-C Port]   │
├─────────────────┤
│ D0  (GPIO2)     │
│ D1  (GPIO3)     │
│►D2  (GPIO4)◄────┼─── DCF77 Signal Output
│ D3  (GPIO5)     │
│ D4  (GPIO6)     │
│ D5  (GPIO7)     │
│ D6  (GPIO21)    │
│ D7  (GPIO20)    │
│ D8  (GPIO8)     │
│ D9  (GPIO9)     │
│ D10 (GPIO10)    │
├─────────────────┤
│ 3V3             │
│►GND◄────────────┼─── Ground
│ 5V              │
│ BAT+            │
│ BAT-            │
└─────────────────┘
```

## 🔌 Connections

### Minimal configuration:
```
XIAO D2 (GPIO4) ──[1kΩ]── BC547 (B)
                           BC547 (E) ── GND
                           BC547 (C) ── Antena ── GND
```

### Full configuration with filtering:
```
XIAO 3V3 ──[100nF]── GND
XIAO D2 ──[1kΩ]── BC547 (B)
          [10kΩ]── GND (optional)
BC547 (E) ── GND
BC547 (C) ── Antena ── GND
```

## 🕐 Sync Hours

Device wakes up automatically:
- **02:00** - transmission 1h
- **03:00** - transmission 1h
- **04:00** - transmission 1h
- **05:00** - transmission 1h

Manual trigger: Press and hold ADJUST button on watch

## 📡 DCF77 Parameters

| Parameter | Value |
|-----------|-------|
| Frequency | 77.5 kHz (emulated) |
| Modulation | AM |
| Bit "0" | 100ms reduction |
| Bit "1" | 200ms reduction |
| Minute marker | 1000ms no signal |
| Range | 5-15 meters |

## 🔋 Power Consumption

| State | Current |
|-------|---------|
| Deep Sleep | 40-50 μA |
| WiFi Active | 100-180 mA |
| Transmitting | 80-120 mA |
| **Average** | **~10-20 mA** |

### Battery Life:
- **500mAh**: ~25-50 hours (1-2 days)
- **1000mAh**: ~50-100 hours (2-4 days)
- **2000mAh**: ~100-200 hours (4-8 days)

## 🛠️ Parts List

### Buy on AliExpress (total ~$6-8 USD):

| Component | Quantity | Price | Search |
|-----------|----------|-------|--------|
| XIAO ESP32C3 | 1 | ~$4 | "Seeeduino XIAO ESP32C3" |
| BC547 NPN | 1-10 | ~$0.25 | "BC547 transistor" |
| Resistor 1kΩ | 1 | ~$0.02 | "1/4W resistor kit" |
| Resistor 10kΩ | 1 | ~$0.02 | "1/4W resistor kit" |
| Capacitor 100nF | 1 | ~$0.02 | "ceramic capacitor" |
| Ferrite rod | 1 | ~$1-2 | "ferrite rod 10mm" |
| Wire 0.3mm | 10m | ~$1 | "enameled copper wire" |

**Total**: ~$6-8 USD

## 🧪 Quick Tests

### Test 1: LED Blink
```cpp
// Upload: examples/simple_gpio_test/
// Expected: LED blinks at 1 Hz
```

### Test 2: WiFi
```cpp
// Upload: examples/wifi_ntp_test/
// Expected: "WiFi connected! 192.168.x.x"
```

### Test 3: Antenna
```
1. Connect LED to GPIO4
2. Upload main code
3. LED blinks once/second = working ✅
```

## 📏 Antenna Dimensions

### Standard:
```
Ferrite rod: 10mm (Ø) × 100mm (L)
Turns: 200
Wire: 0.3mm enameled
Range: 5-10m
```

### Extended range:
```
Ferrite rod: 10mm (Ø) × 200mm (L)
Turns: 300
Wire: 0.4-0.5mm enameled
Range: 10-20m
```

### With MOSFET amplifier:
```
Use IRF540N instead of BC547
+ amplification ~10x
Range: 20-30m
```

## 🎯 Watch Positioning

```
     [Antenna]
         │
         │ 10-30 cm
         ↓
    ┌─────────┐
    │ Casio   │  ← Flat
    │  Watch  │
    └─────────┘
    
✅ GOOD:
- Flat on table
- 10-30 cm from antenna
- Rotation 0°, 90°, 180°, 270° (test each)

❌ BAD:
- Vertical (on side)
- Too far (>50cm)
- Too close (<5cm)
- At an angle
```

## 📞 Support

| Problem | See |
|---------|-----|
| Won't compile | [BUILD.md](BUILD.md) |
| Hardware issues | [HARDWARE.md](HARDWARE.md) |
| Watch won't sync | [TROUBLESHOOTING.md](TROUBLESHOOTING.md) |
| Advanced features | [ADVANCED.md](ADVANCED.md) |
| GitHub Issues | https://github.com/cino893/dcf77-xiao-esp32/issues |

## 🔑 Key Files

```
dcf77-xiao-esp32/
├── dcf77-xiao-esp32.ino    ← Main code
├── config.h                 ← Your WiFi (don't commit!)
├── config.h.example         ← Template
├── README.md                ← Main documentation
├── BUILD.md                 ← How to upload code
├── HARDWARE.md              ← Schematic and assembly
├── TROUBLESHOOTING.md       ← Problem solving
├── ADVANCED.md              ← Advanced features
└── examples/
    ├── simple_gpio_test/    ← GPIO test
    └── wifi_ntp_test/       ← WiFi/NTP test
```

## 🎨 Status LED (optional)

Add LED to GPIO4 for visualization:
```
GPIO4 ──[220Ω]──(LED)──GND
```

Blinking meaning:
- **1 pulse/second**: DCF77 transmission OK ✅
- **No pulses**: Not transmitting (check time)
- **Continuous on**: Code problem

## 🔄 OTA Updates (future)

Planned for next versions:
- Web interface (192.168.x.x)
- WiFi AP mode for configuration
- OTA updates (without USB)
- OLED display
- Sync statistics

## 📚 Links

- **GitHub Repo**: https://github.com/cino893/dcf77-xiao-esp32
- **XIAO Wiki**: https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/
- **DCF77 Info**: https://en.wikipedia.org/wiki/DCF77
- **ESP32 Docs**: https://docs.espressif.com/

---

**Happy hacking!** 🎉
