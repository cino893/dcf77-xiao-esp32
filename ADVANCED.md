# Advanced Features

## 🎯 How DCF77 Works

DCF77 is a radio time signal transmitted from Mainflingen, Germany on 77.5 kHz frequency. The signal consists of:

- **77.5 kHz carrier**: Continuous radio wave
- **Amplitude modulation**: 85% power reduction encodes bits
- **Bit timing**: 
  - Bit "0": 100ms power reduction
  - Bit "1": 200ms power reduction
  - Minute marker: no carrier for entire second
- **Data format**: BCD (Binary Coded Decimal)
- **60 bits/minute**: Complete time information

### Time Encoding

```
Bit 0:    Minute marker (always 0)
Bit 1-14: Weather information and warnings
Bit 15:   Call bit
Bit 16:   Time change announcement
Bit 17:   CEST (summer time)
Bit 18:   CET (standard time)
Bit 19:   Leap second
Bit 20:   Start of time code (always 1)
Bit 21-27: Minutes (BCD)
Bit 28:   Minutes parity
Bit 29-34: Hours (BCD)
Bit 35:   Hours parity
Bit 36-41: Day of month (BCD)
Bit 42-44: Day of week (1-7)
Bit 45-49: Month (BCD)
Bit 50-57: Year (BCD, last 2 digits)
Bit 58:   Date parity
Bit 59:   No bit (preparation for marker)
```

## ⚡ Optimizations

### Extended Range

For range >15m you need:

1. **Better amplifier**:
```
GPIO4 → Gate MOSFET IRF540N
Source → GND
Drain → Antenna + GND through 100Ω resistor
```

2. **Better antenna**:
- Longer ferrite rod (200mm)
- More turns (300-400)
- Better wire (thicker 0.5mm)

3. **Resonance at 77.5 kHz**:
- Add parallel capacitor to antenna
- Calculate: C = 1 / (4π²f²L)
- For L=3mH, f=77.5kHz → C ≈ 1.4nF

### More Accurate Frequency

**✅ IMPLEMENTED (v2.0)**: Current implementation uses PWM for amplitude control!

PWM implementation for amplitude modulation is already available in the code:

```cpp
// In config.h:
#define DCF77_PWM_MODE true          // Enable PWM mode
#define DCF77_AMPLITUDE_LOW 51       // 20% amplitude for Casio
#define DCF77_AMPLITUDE_HIGH 0       // 0% amplitude (no signal)
#define DCF77_PWM_FREQUENCY 2000     // 2 kHz PWM for amplitude control
```

**How it works**:
- PWM (2 kHz) controls average signal amplitude
- 20% duty cycle = ~20% power for LOW signal
- 0% duty cycle = no signal for HIGH
- Important for Casio watches!

**See details**: [DCF77_SIGNAL_LEVELS.md](DCF77_SIGNAL_LEVELS.md)

### Future Improvements

1. **True 77.5 kHz carrier** (for even better range):
```cpp
// Generate 77.5 kHz carrier (optional)
ledcSetup(1, 77500, 8);  // Channel 1, 77.5kHz carrier
ledcAttachPin(DCF77_PIN, 1);
ledcWrite(1, 128);  // 50% duty cycle
```

2. **Hardware timer**:
- ESP32 has hardware timers
- Can generate exactly 77.5 kHz

## 🔍 Testing

### Testing Without a Watch

1. **LED indicator**: Connect LED to GPIO4 through 220Ω resistor
2. **Serial monitor**: Observe transmitted bits
3. **Oscilloscope**: Check timing (100ms/200ms)
4. **AM radio**: AM receiver on ~77 kHz can detect signal

### Debug Transmission

In serial monitor you should see:
```
M0100000100 0000011000 1000100001 0001100100 0001000101 0100100
```

Where:
- `M` = Minute marker
- Following digits = consecutive DCF77 bits
- Spaces every 10 bits for readability

## 🌍 Other Time Radio Systems

This code can be adapted for:

### WWVB (USA) - 60 kHz
- Similar to DCF77
- Timing: 200ms=0, 500ms=1, 800ms=marker
- Change frequency and data format

### MSF (UK) - 60 kHz  
- Similar timing to WWVB
- Different encoding format

### JJY (Japan) - 40/60 kHz
- Two frequencies
- Similar format to WWVB

## 📱 Mobile App (Future)

Possible extensions:
- ESP32 as access point
- Web configuration interface
- Android/iOS control app
- Bluetooth LE for configuration

## 🔐 Security

⚠️ **Important legal notes**:

1. **Transmission power**: Keep low power (<100mW)
2. **Personal use**: Only for synchronizing your own watches
3. **Range**: Limited to your own home
4. **Local regulations**: Check local RF device regulations
5. **Don't interfere**: Don't cause interference in the 77.5 kHz band

## 💡 Tips

### Best Synchronization Times
Casio automatically synchronizes:
- **2:00-4:00 AM**: Main synchronization window
- **5:00 AM**: Additional attempt (some models)

### Watch Positioning
- Place watch **flat** (watch antenna is horizontal)
- Distance: **10-30 cm** from antenna
- Orientation: Try rotating watch 90° if not receiving
- Electrical silence: Turn off phone, laptop during sync

### Antenna Optimization
- **Experiment with turns**: 150-300
- **Tight winding**: Turns close together
- **Center of rod**: Wind on middle section
- **Wire quality**: Enameled copper wire

## 🛠️ Project Development

### TODO List:
- [ ] PWM 77.5 kHz for better modulation
- [ ] Web interface for configuration
- [ ] Battery support with level meter
- [ ] Automatic timezone detection
- [ ] Support for WWVB/MSF/JJY
- [ ] OLED display with status
- [ ] OTA (Over The Air) updates
- [ ] Synchronization statistics

### Contribution
Pull requests welcome! Suggestions:
- Documentation translations
- New features
- Range optimizations
- Tests with different watches
- PCB schematics

## 📞 Support

Problems? Issues:
- GitHub Issues: https://github.com/cino893/dcf77-xiao-esp32/issues
- Describe problem in detail
- Attach logs from serial monitor
- Hardware information

## 🙏 Credits

Project inspired by:
- DCF77 library by Udo Klein
- ESP32 Arduino Core by Espressif
- Community contributions

## 📚 References

### DCF77 Protocol
- [PTB Official DCF77 Info](https://www.ptb.de/cms/en/ptb/fachabteilungen/abt4/fb-44/ag-442/dissemination-of-legal-time/dcf77.html)
- [DCF77 Wikipedia](https://en.wikipedia.org/wiki/DCF77)
- [DCF77 Decoding Guide](http://www.dcf77logs.de/live)

### ESP32 Resources
- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)
- [XIAO ESP32C3 Wiki](https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/)
- [ESP32 Technical Reference](https://www.espressif.com/sites/default/files/documentation/esp32-c3_technical_reference_manual_en.pdf)

### Time Synchronization
- [NTP Protocol](https://en.wikipedia.org/wiki/Network_Time_Protocol)
- [Leap Seconds](https://www.nist.gov/pml/time-and-frequency-division/leap-seconds-faqs)

---

**Have fun with the project!** 🎉
