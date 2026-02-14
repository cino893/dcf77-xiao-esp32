# Hardware Schematic

## Components

### Essential:
1. **Seeed Studio XIAO ESP32C3** - main microcontroller
   - ~3.5 USD on AliExpress
2. **NPN Transistor (BC547 or 2N2222)** - signal amplifier
   - ~0.02 USD
3. **1kΩ Resistor** - base current limiter
   - ~0.01 USD
4. **10kΩ Resistor** - pull-down resistor
   - ~0.01 USD
5. **Ferrite rod antenna 77.5kHz** - transmit antenna
   - Can use:
     - Ferrite rod 10mm x 100mm with wire coil
     - ~200 turns of wire (0.3mm)
     - Or ready AM antenna
   - ~1-5 USD on AliExpress
6. **100nF Capacitor** - power filtering
   - ~0.02 USD

### Optional for extended range:
7. **MOSFET Amplifier (IRF540N)** - higher output power
   - ~0.50 USD
8. **77.5kHz Crystal Resonator** - more accurate frequency
   - ~2-5 USD

## Circuit Diagram

### Basic Circuit:

```
                                    +3.3V
                                      |
                                      |
                                     [R2]  10kΩ
                                      |
    XIAO ESP32C3                      |
    ┌──────────────┐                  |
    │              │                  |
    │   D2 (GPIO4) ├────[R1]──────────┤
    │              │   1kΩ         B  │ Q1
    │          GND ├────────────────┼──┤ BC547/2N2222
    │              │              E │  │ NPN
    │          3V3 ├──────┬─────────┘  │
    │              │      │         C  │
    └──────────────┘      │            │
                          │            │
                         [C1]          │
                        100nF          │
                          │            │
                         GND        [ANTENNA]
                                   Ferrite coil
                                   ~200 turns
                                   0.3mm wire
                                      │
                                     GND

R1 = 1kΩ (base resistor)
R2 = 10kΩ (pull-down resistor)
C1 = 100nF (filter capacitor)
Q1 = BC547 or 2N2222 (NPN transistor)
```

### How it works:

1. **Pin D2 (GPIO4)** emits the DCF77 signal (amplitude modulation)
   - HIGH = full transmission power
   - LOW = reduced power (carrier simulation)

2. **Transistor Q1** amplifies the signal:
   - When GPIO4 = HIGH → transistor conducts → current flows through antenna
   - When GPIO4 = LOW → transistor off → no current in antenna

3. **Ferrite antenna** emits magnetic field:
   - Modulated magnetic field at ~77.5kHz frequency
   - Range: 5-10 meters (depends on antenna)

## Antenna Assembly

### Option 1: DIY Antenna
```
Ferrite rod
10mm diameter × 100mm length

    ╔═══════════════════════════════════╗
    ║  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ║  ← 200 turns of 0.3mm wire
    ║  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ║
    ╚═══════════════════════════════════╝

    └── Start of coil to Q1 collector
    
    └── End of coil to GND
```

Parameters:
- Wire: 0.3mm enameled
- Turns: ~200 (experiment with 150-250)
- Inductance: ~2-5 mH
- Wind evenly on the middle section of the rod

### Option 2: Ready AM Antenna
- Use antenna from AM/MW radio
- Resonant frequency ~500-1600kHz is OK
- Range may be greater

## Power Supply

**Important!** XIAO ESP32C3 requires stable 3.3V or 5V (USB):

1. **USB (5V)** - simplest, via USB-C port
2. **Li-Po Battery 3.7V** - for portable mode
   - Connect to BAT+ and BAT- pins
   - XIAO has built-in charger
3. **5V Power Supply** - via 5V pin

### Power Consumption:
- Active (WiFi ON): ~100-180mA
- Transmitting (WiFi OFF): ~80-120mA  
- Deep Sleep: ~40-50μA
- Average: ~10-20mA (with deep sleep)

## Range

Expected range:
- **Basic circuit**: 3-7 meters
- **With better antenna**: 5-15 meters
- **With MOSFET amplifier**: 10-30 meters

Range factors:
- Antenna quality
- Antenna orientation to watch
- Metal objects nearby
- Signal strength (collector current)

## Debugging

### LED Test:
You can add an LED to monitor the signal:

```
GPIO4 ────[220Ω]────(LED)────GND
```

The LED will blink according to the DCF77 signal (1 pulse/second).

## Safety Notes

⚠️ **Important:**
- Do not exceed 100mW transmit power (regulations)
- Use only for personal purposes (synchronizing your own watches)
- Do not interfere with other devices
- Keep safe distance from antenna (>10cm)

## AliExpress Shopping List

Search for:
1. "XIAO ESP32C3" or "Seeeduino XIAO ESP32C3"
2. "BC547 transistor NPN" or "2N2222"
3. "Ferrite rod antenna" or "AM antenna ferrite rod"
4. "Resistor kit 1/4W"
5. "Ceramic capacitor kit"
6. "0.3mm enameled copper wire"

Total cost: ~5-8 USD
