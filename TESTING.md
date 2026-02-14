# Testing Guide for DCF77 Signal Implementation

This document provides guidance on testing the DCF77 signal implementation, particularly the PWM amplitude modulation feature for Casio compatibility.

## Quick Verification Tests

### Test 1: Serial Monitor Verification

**Purpose**: Verify that PWM mode is enabled and configured correctly.

**Steps**:
1. Upload the code to XIAO ESP32C3
2. Open Serial Monitor (115200 baud)
3. Press the RESET button on XIAO
4. Look for the configuration output

**Expected Output**:
```
=== DCF77 Emulator for XIAO ESP32C3 ===
Boot count: 1
Configuring DCF77 output...
  Mode: 77.5 kHz PWM carrier with amplitude modulation
  Carrier frequency: 77500 Hz (77.5 kHz)
  LOW amplitude: 51 (~20.0% duty cycle = reduced carrier)
  HIGH amplitude: 0 (~0.0% duty cycle = no carrier)
```

**Pass Criteria**:
- ✅ Mode shows "77.5 kHz PWM carrier with amplitude modulation" (not "Simple GPIO on/off")
- ✅ Carrier frequency is 77500 Hz (77.5 kHz)
- ✅ LOW amplitude is 51 (or your configured value)
- ✅ HIGH amplitude is 0

**If Test Fails**:
- Check that `config.h` has `#define DCF77_PWM_MODE true`
- Verify `config.h` is in the same directory as `.ino` file
- Re-upload the code

---

### Test 2: Transmission Verification

**Purpose**: Verify that DCF77 signal is being transmitted with correct timing.

**Steps**:
1. Wait for transmission to start (during sync hours 2-5 AM, or force manual transmission)
2. Observe serial monitor output

**Expected Output**:
```
=== Transmitting DCF77 signal for 02:34 ===
Encoded time: 02:35 13.02.2026 (Day 5)
M0100000100 0000011000 1000100001 0001100100 0001000101 0100100
```

**Pass Criteria**:
- ✅ First character is 'M' (minute marker)
- ✅ 59 additional bits (0s and 1s) following the marker
- ✅ Bits are printed at 1-second intervals
- ✅ Transmission completes in exactly 60 seconds

**Timing Check**:
- Each bit should take exactly 1 second
- Use a stopwatch to verify: 60 bits = 60 seconds total

---

### Test 3: LED Indicator Test

**Purpose**: Visual verification of signal amplitude modulation.

**Required Hardware**:
- LED (any color, 3mm or 5mm)
- 220Ω resistor
- Breadboard or jumper wires

**Connections**:
```
GPIO4 (D2) → 220Ω Resistor → LED Anode (+, longer leg)
LED Cathode (-, shorter leg) → GND
```

**Expected Behavior**:
- **PWM Mode (with 20% LOW amplitude)**:
  - LED should show brief dim pulses (100ms or 200ms)
  - Followed by LED being completely off
  - Minute marker: 1 second of dim light
  
- **GPIO Mode (legacy)**:
  - LED fully on during pulses
  - LED off between pulses

**Pass Criteria**:
- ✅ LED pulses occur once per second
- ✅ Two different pulse lengths visible (100ms for 0, 200ms for 1)
- ✅ In PWM mode, LED brightness is noticeably reduced compared to full brightness

---

### Test 4: Casio Watch Synchronization Test

**Purpose**: Real-world test with Casio multiband watch.

**Prerequisites**:
- Casio multiband watch (Wave Ceptor, G-Shock Multiband 6, etc.)
- Assembled antenna circuit

**Steps**:
1. Set current time on ESP32 via NTP (ensure accurate time)
2. Place Casio watch flat on a table
3. Position antenna 10-30 cm above watch (vertical orientation)
4. Start DCF77 transmission during sync hours (2-5 AM) or manually trigger sync on watch
5. Wait 2-10 minutes for watch to synchronize

**Manual Sync Trigger** (varies by model):
- Press and hold ADJUST button for 2-3 seconds
- Watch should display "RCV" or similar reception indicator
- Some models: Hold MODE + LIGHT buttons

**Expected Behavior**:
- Watch displays reception indicator (antenna symbol, "RCV", etc.)
- After 2-10 minutes: Watch time updates to match transmitted time
- Reception indicator shows success

**Pass Criteria**:
- ✅ Watch enters receive mode
- ✅ Watch successfully synchronizes within 10 minutes
- ✅ Watch time matches ESP32 time after sync

**If Test Fails**:
1. Verify PWM mode is active (Test 1)
2. Try different watch orientations (rotate 90°, 180°, 270°)
3. Adjust distance (try 15 cm, 25 cm, 40 cm)
4. Check antenna quality (should have ~200 turns)
5. Try different amplitude values (45, 51, 58, 64)
6. See [TROUBLESHOOTING.md](TROUBLESHOOTING.md) Step 7

---

## Advanced Testing (Optional)

### Test 5: Oscilloscope Measurement

**Purpose**: Verify PWM waveform and timing accuracy.

**Required Equipment**:
- Oscilloscope (100 kHz+ bandwidth)
- 10:1 probe

**Measurement Points**:
1. **GPIO4 (XIAO ESP32C3 Pin D2)**
2. **Transistor collector** (antenna connection)

**Expected Waveforms**:

#### At GPIO4 During LOW Amplitude (100ms or 200ms pulse):
```
Channel 1 (GPIO4):
Time scale: 5 μs/div (to see 77.5 kHz waveform)
Voltage scale: 1 V/div

Expected:
- PWM waveform at 77.5 kHz (12.9 μs period)
- Duty cycle: ~20% (2.6 μs high, 10.3 μs low)
- High voltage: 3.3V
- Low voltage: 0V
- This creates reduced amplitude 77.5 kHz carrier
```

#### At GPIO4 During HIGH Amplitude (rest of second):
```
Channel 1 (GPIO4):
Time scale: 100 ms/div
Voltage scale: 1 V/div

Expected:
- Constant 0V
- No carrier (no PWM pulses)
```

#### Timing Verification:
```
Channel 1 (GPIO4):
Time scale: 100 ms/div

Measure:
- LOW amplitude duration for bit 0: 100 ms (±1 ms)
- LOW amplitude duration for bit 1: 200 ms (±1 ms)
- Total bit period: 1000 ms (±2 ms)
- Minute marker: 1000 ms of LOW amplitude
```

**Pass Criteria**:
- ✅ PWM frequency is 77500 Hz (12.9 μs period) ±5%
- ✅ PWM duty cycle is 20% ±2%
- ✅ Bit timing is accurate within 2ms
- ✅ HIGH amplitude is stable 0V

---

### Test 6: Amplitude Adjustment Test

**Purpose**: Find optimal amplitude for specific Casio model.

**Steps**:
1. Start with default `DCF77_AMPLITUDE_LOW 51` (20%)
2. If watch doesn't sync after 10 minutes:
3. Try increasing: `DCF77_AMPLITUDE_LOW 64` (25%)
4. Upload and test for 10 minutes
5. If still fails, try decreasing: `DCF77_AMPLITUDE_LOW 38` (15%)
6. Upload and test for 10 minutes
7. Continue with: 45 (18%), 58 (23%), 70 (27%)

**Recommended Test Sequence**:
```cpp
// Test in this order:
#define DCF77_AMPLITUDE_LOW 51    // 20% - standard (try first)
#define DCF77_AMPLITUDE_LOW 64    // 25% - slightly higher
#define DCF77_AMPLITUDE_LOW 45    // 18% - slightly lower
#define DCF77_AMPLITUDE_LOW 38    // 15% - minimum
#define DCF77_AMPLITUDE_LOW 58    // 23% - mid-high
#define DCF77_AMPLITUDE_LOW 70    // 27% - maximum (last resort)
```

**Record Results**:
- Document which amplitude works for your watch model
- Share findings in project issues or discussions
- See [DCF77_SIGNAL_LEVELS.md](DCF77_SIGNAL_LEVELS.md) for contribution instructions

---

## Test Results Template

Copy this template to document your test results:

```markdown
### My Test Results

**Date**: YYYY-MM-DD
**Hardware**:
- ESP32 Board: XIAO ESP32C3
- Transistor: BC547 / 2N2222 / Other: _____
- Antenna: _____ turns, _____ mm diameter, _____ mm length
- Wire: _____ mm diameter

**Software Configuration**:
- PWM Mode: Enabled / Disabled
- LOW Amplitude: _____ (___%)
- HIGH Amplitude: _____ (___%)
- PWM Frequency: _____ Hz

**Casio Watch**:
- Model: _____________________
- Firmware/Version: _____

**Test Results**:
- [ ] Test 1: Serial Monitor - Pass / Fail
- [ ] Test 2: Transmission - Pass / Fail
- [ ] Test 3: LED Indicator - Pass / Fail
- [ ] Test 4: Watch Sync - Pass / Fail (Time: ___ minutes)

**Watch Positioning**:
- Distance from antenna: _____ cm
- Orientation: Flat / Angled / Vertical
- Best angle: 0° / 90° / 180° / 270°

**Notes**:
_____________________
_____________________
```

---

## Troubleshooting Failed Tests

### Test 1 Fails: Wrong Mode Displayed

**Problem**: Serial monitor shows "Simple GPIO on/off" instead of "PWM amplitude modulation"

**Solutions**:
1. Check `config.h` exists in project directory
2. Verify `#define DCF77_PWM_MODE true` is in config.h
3. Check for typos in macro name
4. Re-upload code
5. Press RESET button after upload

---

### Test 2 Fails: No Transmission

**Problem**: No "Transmitting DCF77 signal" message appears

**Solutions**:
1. Check current time is correctly set via NTP
2. Verify WiFi is connected (check serial output)
3. Check if current hour is in sync hours (2-5 AM)
4. Modify sync hours in code to include current hour for testing
5. Check NTP synchronization was successful

---

### Test 3 Fails: LED Always On or Always Off

**Problem**: LED doesn't show expected pulsing behavior

**Solutions**:
1. Check LED polarity (anode to resistor, cathode to GND)
2. Verify resistor value (220Ω recommended, 100-1000Ω acceptable)
3. Test LED separately (connect to 3.3V through resistor)
4. Check GPIO4 connection
5. Try different LED (may be damaged)

---

### Test 4 Fails: Watch Won't Sync

**Problem**: Casio watch doesn't synchronize even after 15 minutes

**Solutions**:
1. **Verify PWM mode is active** (Test 1 must pass)
2. **Check amplitude settings**: Try 51, then 64, then 38
3. **Antenna quality**:
   - Verify ~200 turns of wire
   - Check connections (no breaks)
   - Ensure tight, even winding
4. **Positioning**:
   - Try 4 different watch orientations (rotate 90° each time)
   - Test distances: 10cm, 20cm, 30cm
   - Keep watch flat on surface
5. **Interference**:
   - Turn off nearby electronics
   - Move away from computer, phone
   - Try in different room
6. **Time accuracy**:
   - Verify ESP32 time is correct
   - Check timezone settings
   - Ensure NTP sync succeeded
7. **Signal level tuning**:
   - See Test 6 for amplitude adjustment procedure
   - See [TROUBLESHOOTING.md](TROUBLESHOOTING.md) Step 7

---

## Continuous Integration Testing (For Developers)

### Automated Syntax Check

```bash
# Check .ino file syntax
python3 -c "
import re
with open('dcf77-xiao-esp32.ino', 'r') as f:
    content = f.read()
    # Check for basic syntax errors
    if content.count('{') != content.count('}'):
        print('ERROR: Mismatched braces')
        exit(1)
    if content.count('(') != content.count(')'):
        print('ERROR: Mismatched parentheses')
        exit(1)
    print('Basic syntax check: PASS')
"
```

### Configuration Validation

```bash
# Verify config.h has required definitions
grep -q "DCF77_PWM_MODE" config.h && echo "✓ DCF77_PWM_MODE defined" || echo "✗ Missing DCF77_PWM_MODE"
grep -q "DCF77_AMPLITUDE_LOW" config.h && echo "✓ DCF77_AMPLITUDE_LOW defined" || echo "✗ Missing DCF77_AMPLITUDE_LOW"
grep -q "DCF77_AMPLITUDE_HIGH" config.h && echo "✓ DCF77_AMPLITUDE_HIGH defined" || echo "✗ Missing DCF77_AMPLITUDE_HIGH"
grep -q "DCF77_PWM_FREQUENCY" config.h && echo "✓ DCF77_PWM_FREQUENCY defined" || echo "✗ Missing DCF77_PWM_FREQUENCY"
```

---

## References

- [DCF77_SIGNAL_LEVELS.md](DCF77_SIGNAL_LEVELS.md) - Detailed signal level documentation
- [TROUBLESHOOTING.md](TROUBLESHOOTING.md) - Common problems and solutions
- [HARDWARE.md](HARDWARE.md) - Circuit diagrams and antenna construction
- [ADVANCED.md](ADVANCED.md) - Advanced modifications and optimizations

---

**Last Updated**: 2026-02-13
**Version**: 2.0 (PWM implementation)
