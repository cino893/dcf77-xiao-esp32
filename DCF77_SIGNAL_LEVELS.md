# DCF77 Signal Levels and Casio Compatibility

## Overview

This document explains the DCF77 signal amplitude requirements, particularly for Casio multiband watch compatibility, and provides guidance on testing and measurement.

## DCF77 Signal Specification

### Standard DCF77 Amplitude Modulation

The DCF77 time signal uses **Amplitude Modulation (AM)** to encode binary data:

- **Carrier Frequency**: 77.5 kHz
- **Modulation Type**: AM with amplitude reduction
- **Standard Specification** (PTB, Germany):
  - **Carrier "ON"** (reduced power): ~15-25% of full amplitude
  - **Carrier "OFF"** (full power or no signal): 0% or 100% depending on implementation

### Bit Encoding Timing

- **Bit "0"**: 100ms of reduced amplitude, followed by 900ms of full/no signal
- **Bit "1"**: 200ms of reduced amplitude, followed by 800ms of full/no signal  
- **Minute Marker**: 1000ms (full second) of reduced amplitude, no full signal period

## Casio Multiband Watch Requirements

### Signal Level Requirements

Casio multiband watches are particularly sensitive to the amplitude modulation levels:

1. **Low Amplitude** (carrier reduction):
   - **Required Range**: 15-25% of maximum amplitude
   - **Recommended**: ~20% (51/255 in 8-bit PWM)
   - **Critical**: Without this level, Casio watches **cannot detect** the signal modulation

2. **High Amplitude** (carrier off):
   - **Required**: 0% (no signal)
   - **Purpose**: Creates the contrast needed for Casio's detector circuitry

### Why Casio Devices Are Sensitive

Casio uses a specific detection circuit that:
- Measures the **difference** between high and low amplitude levels
- Requires a clear 15-25% "dip" in amplitude to register as a valid DCF77 bit
- Will **fail to synchronize** if the amplitude reduction is too small or too large
- May not lock if simple on/off GPIO switching is used without proper amplitude control

### Tested Casio Models

The following Casio models are known to work with proper amplitude modulation:
- Casio Wave Ceptor series
- Casio G-Shock Multiband 6
- Casio Oceanus
- Casio ProTrek

## Implementation Modes

### PWM Mode (Recommended for Casio)

**Configuration**: Set `DCF77_PWM_MODE = true` in `config.h`

This mode uses ESP32's hardware PWM (LEDC peripheral) to generate a 77.5 kHz carrier with amplitude modulation:

```cpp
#define DCF77_PWM_MODE true
#define DCF77_AMPLITUDE_LOW 51           // ~20% duty cycle (reduced carrier)
#define DCF77_AMPLITUDE_HIGH 0           // 0% duty cycle (no carrier)
#define DCF77_CARRIER_FREQUENCY 77500    // 77.5 kHz carrier frequency
```

**Advantages**:
- Generates actual 77.5 kHz carrier wave
- Precise amplitude control via duty cycle modulation
- Adjustable to match specific watch requirements
- Better Casio compatibility
- Can fine-tune for optimal reception

**How it works**:
- PWM at 77.5 kHz generates the DCF77 carrier frequency
- Duty cycle controls carrier amplitude (AM modulation):
  - 0% duty cycle = no carrier (carrier off)
  - 20% duty cycle = reduced amplitude carrier (for Casio AM detection)
  - 50% duty cycle = full amplitude carrier (square wave)
- During 100ms/200ms pulses: 20% duty cycle transmits reduced carrier
- Rest of second: 0% duty cycle turns carrier completely off
- This creates the amplitude modulation that Casio watches detect

### GPIO Mode (Legacy)

**Configuration**: Set `DCF77_PWM_MODE = false` in `config.h`

Simple on/off switching of GPIO pin:

```cpp
#define DCF77_PWM_MODE false
```

**Limitations**:
- Does NOT generate 77.5 kHz carrier
- Only two levels: full on or full off
- May not provide proper DCF77 signal
- Relies entirely on antenna resonance to create RF field
- Works with some receivers but not reliable for Casio devices

## Signal Testing and Measurement

### Testing Without Oscilloscope

#### Method 1: Casio Watch Reception Test

1. **Setup**:
   - Place Casio watch flat, 10-30 cm from antenna
   - Ensure watch is in receive mode (usually midnight or manual sync)
   - Room should be quiet (minimal electrical interference)

2. **Test Procedure**:
   - Start DCF77 transmission
   - Watch for the reception indicator on the watch
   - Successful reception typically takes 2-10 minutes

3. **Troubleshooting**:
   - **No reception**: Check amplitude settings (try 51-64 for LOW)
   - **Intermittent**: Adjust watch orientation or distance
   - **Never locks**: Verify PWM mode is enabled

#### Method 2: LED Indicator Test

1. Connect an LED (with 220Ω resistor) to GPIO4
2. Observe the LED brightness during transmission:
   - Should see dim pulses (100ms or 200ms) followed by LED off
   - Minute marker: continuous dim light for 1 second

#### Method 3: Serial Monitor Verification

Monitor the serial output during transmission:
```
Configuring DCF77 output...
  Mode: 77.5 kHz PWM carrier with amplitude modulation
  Carrier frequency: 77500 Hz (77.5 kHz)
  LOW amplitude: 51 (~20.0% duty cycle = reduced carrier)
  HIGH amplitude: 0 (~0.0% duty cycle = no carrier)

Transmitting DCF77 signal...
M0100000100 0000011000 1000100001 0001100100 0001000101 0100100
```

- `M` = Minute marker
- `0` = 100ms reduced carrier pulse
- `1` = 200ms reduced carrier pulse

### Testing With Oscilloscope

#### Setup Requirements

- **Oscilloscope**: 100 kHz+ bandwidth (minimum - 500 kHz recommended for clear 77.5 kHz)
- **Probe**: 10:1 probe recommended
- **Connection**: Measure at GPIO4 or transistor collector (antenna connection)

#### Expected Waveforms

##### PWM Mode (Correct Configuration)

**During "LOW" amplitude (100ms or 200ms pulse)**:
```
Voltage at GPIO4 (77.5 kHz carrier at 20% duty cycle):
    3.3V ┐  ┌┐  ┌┐  ┌┐  ┌┐  ┌┐  ┌┐  ┌┐  
         │  ││  ││  ││  ││  ││  ││  ││  
    0V   └──┘└──┘└──┘└──┘└──┘└──┘└──┘└──
         └─12.9µs─┘ (77.5 kHz period)
         
High time: ~2.6µs (20% of 12.9µs)
Low time: ~10.3µs (80% of 12.9µs)
Result: Reduced amplitude 77.5 kHz carrier
```

**During "HIGH" amplitude (rest of second)**:
```
Voltage at GPIO4:
    3.3V
         
    0V  ──────────────────────────  (Constant 0V = no carrier)
```

##### Antenna Output

The antenna/transistor collector will show the 77.5 kHz carrier modulated by the amplitude changes:
- During 100ms/200ms pulses: 77.5 kHz carrier at reduced amplitude (20% duty cycle)
- Rest of second: No carrier (0V)
- Carrier stops (or continues at full) for rest of second

#### Measurements to Verify

1. **PWM Frequency**: Should be 2000 Hz (2 kHz)
2. **PWM Duty Cycle**: Should be ~20% during LOW amplitude
3. **Pulse Duration**: 
   - 100ms for bit "0"
   - 200ms for bit "1"
   - 1000ms for minute marker
4. **Timing Accuracy**: Within ±1ms

### Testing With AM Radio

Many AM radios can detect the 77.5 kHz signal:

1. **Setup**:
   - Tune AM radio to lowest frequency (around 500-600 kHz)
   - Place near antenna
   - Volume up

2. **Expected Sound**:
   - Series of clicks/pops once per second
   - Different lengths for 0 and 1 bits
   - One long click for minute marker

3. **Interpretation**:
   - If you hear regular clicks, signal is being transmitted
   - If no clicks, check hardware connections
   - If irregular, check timing in code

## Configuration Guide

### For Casio Watches (Recommended Settings)

```cpp
// config.h
#define DCF77_PWM_MODE true
#define DCF77_AMPLITUDE_LOW 51      // 20% - standard Casio
#define DCF77_AMPLITUDE_HIGH 0      // 0% - no signal
#define DCF77_PWM_FREQUENCY 2000    // 2 kHz
```

### Adjustment for Different Casio Models

Some Casio models may require adjustment:

**If watch doesn't sync**:
1. Try increasing LOW amplitude: `DCF77_AMPLITUDE_LOW 64` (25%)
2. Try decreasing: `DCF77_AMPLITUDE_LOW 38` (15%)
3. Adjust PWM frequency: `DCF77_PWM_FREQUENCY 1000` (1 kHz)

**If watch syncs but takes very long**:
1. Increase antenna power (better transistor, more coil turns)
2. Reduce distance between antenna and watch
3. Minimize electrical interference

### For Non-Casio Receivers

Many generic DCF77 receivers are less sensitive and may work with GPIO mode:

```cpp
#define DCF77_PWM_MODE false
```

Or with higher amplitude:

```cpp
#define DCF77_PWM_MODE true
#define DCF77_AMPLITUDE_LOW 128     // 50% amplitude
```

## Troubleshooting

### Watch Never Synchronizes

**Possible Causes**:
1. Wrong amplitude levels → Try PWM mode with 20% LOW
2. Hardware issue → Check transistor and antenna connections
3. Timing issue → Verify NTP sync is working
4. Interference → Turn off nearby electronics
5. Watch orientation → Try rotating watch 90°

### Intermittent Synchronization

**Possible Causes**:
1. Amplitude on threshold → Fine-tune LOW amplitude (45-60)
2. Weak signal → Improve antenna (more turns, better positioning)
3. Interference → Identify and remove interference sources
4. Distance too great → Move watch closer

### Strong Signal but No Sync

**Possible Causes**:
1. Incorrect bit encoding → Verify time and parity calculations
2. Timing drift → Check second boundaries are accurate
3. Missing minute marker → Ensure second 0 is transmitted correctly

## Hardware Considerations

### Antenna Quality Impact

The antenna significantly affects amplitude modulation:

**Poor Antenna** (< 150 turns):
- Weak signal strength
- May require higher PWM amplitude
- Reduced range

**Good Antenna** (200-250 turns):
- Strong signal
- Standard 20% amplitude works well
- 5-15 meter range

**Excellent Antenna** (300+ turns, resonant circuit):
- Very strong signal
- May need to reduce amplitude
- 20-30 meter range

### Transistor Selection

Different transistors affect amplitude control:

**BC547 (Standard)**:
- hFE ~100-200
- Works well with PWM control
- Recommended for most applications

**2N2222**:
- Higher current capability
- Good for stronger signals
- Slightly less linear

**MOSFET (IRF540N)**:
- Very high power
- May need amplitude adjustment
- Best for maximum range

## References

### Official DCF77 Documentation

- [PTB Official DCF77 Specification](https://www.ptb.de/cms/en/ptb/fachabteilungen/abt4/fb-44/ag-442/dissemination-of-legal-time/dcf77.html)
- [DCF77 Wikipedia](https://en.wikipedia.org/wiki/DCF77)

### Casio Multiband Information

- Casio Multiband 6 watches typically sync at 2-5 AM local time
- Require 2-10 minutes of stable signal for full synchronization
- Best reception when watch is placed flat with 12 o'clock pointing north

### Technical Background

- AM Modulation: Amplitude is varied to encode information
- PWM (Pulse Width Modulation): Average voltage control technique
- LEDC (LED Control): ESP32 hardware PWM peripheral

## Version History

- **v1.0** (2024): Initial GPIO-only implementation
- **v2.0** (2026): Added PWM mode for Casio compatibility

## Contributing

If you have successfully synchronized a Casio watch with specific settings, please share:
- Casio model name
- Amplitude settings used (LOW/HIGH)
- PWM frequency
- Antenna specifications
- Distance from antenna

This helps improve compatibility for all users.

---

For additional help, see:
- [README.md](README.md) - General setup instructions
- [HARDWARE.md](HARDWARE.md) - Circuit diagrams and component list  
- [TROUBLESHOOTING.md](TROUBLESHOOTING.md) - Common problems and solutions
- [ADVANCED.md](ADVANCED.md) - Advanced modifications
