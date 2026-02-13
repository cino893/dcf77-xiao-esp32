# Schemat Hardware / Hardware Schematic

## Komponenty / Components

### Podstawowe / Essential:
1. **Seeed Studio XIAO ESP32C3** - główny mikrokontroler / main microcontroller
   - ~15 PLN / ~3.5 USD na AliExpress
2. **Tranzystor NPN (BC547 lub 2N2222)** - wzmacniacz sygnału / signal amplifier
   - ~0.10 PLN / ~0.02 USD
3. **Rezystor 1kΩ** - ograniczenie prądu bazy tranzystora / base current limiter
   - ~0.05 PLN / ~0.01 USD
4. **Rezystor 10kΩ** - pull-down / pull-down resistor
   - ~0.05 PLN / ~0.01 USD
5. **Cewka ferrytowa (ferrite rod antenna) 77.5kHz** - antena nadawcza / transmit antenna
   - Można użyć / Can use:
     - Ferrite rod 10mm x 100mm z drutem nawojowym / with wire coil
     - ~200 zwojów drutu / ~200 turns of wire (0.3mm)
     - Lub gotowa antena AM / Or ready AM antenna
   - ~5-20 PLN / ~1-5 USD na AliExpress
6. **Kondensator 100nF** - filtrowanie zasilania / power filtering
   - ~0.10 PLN / ~0.02 USD

### Opcjonalne dla większego zasięgu / Optional for extended range:
7. **Wzmacniacz MOSFET (IRF540N)** - większa moc wyjściowa / higher output power
   - ~2 PLN / ~0.50 USD
8. **Rezonator kwarcowy 77.5kHz** - dokładniejsza częstotliwość / more accurate frequency
   - ~10-20 PLN / ~2-5 USD

## Schemat połączeń / Circuit Diagram

### Podstawowy układ / Basic Circuit:

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
                         GND        [ANTENA]
                                   Ferrite coil
                                   ~200 zwojów
                                   0.3mm drut
                                      │
                                     GND

R1 = 1kΩ (rezystor bazy / base resistor)
R2 = 10kΩ (pull-down resistor)
C1 = 100nF (kondensator filtrujący / filter capacitor)
Q1 = BC547 lub 2N2222 (tranzystor NPN / NPN transistor)
```

### Opis działania / How it works:

1. **Pin D2 (GPIO4)** emituje sygnał DCF77 (modulacja amplitudy)
   - HIGH = pełna moc nadawania / full transmission power
   - LOW = zredukowana moc (symulacja nośnej) / reduced power (carrier simulation)

2. **Tranzystor Q1** wzmacnia sygnał:
   - Kiedy GPIO4 = HIGH → tranzystor przewodzi → prąd płynie przez antenę
   - Kiedy GPIO4 = LOW → tranzystor zatkany → brak prądu w antenie

3. **Antena ferrytowa** emituje pole magnetyczne:
   - Modulowane pole magnetyczne na częstotliwości ~77.5kHz
   - Zasięg: 5-10 metrów (zależy od anteny)

## Montaż anteny / Antenna Assembly

### Opcja 1: Własna antena / DIY Antenna
```
Ferrite rod (pręt ferrytowy)
10mm średnica × 100mm długość

    ╔═══════════════════════════════════╗
    ║  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ║  ← 200 zwojów drutu 0.3mm
    ║  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ║     200 turns of 0.3mm wire
    ╚═══════════════════════════════════╝

    └── Początek cewki do kolektora Q1
        Start of coil to Q1 collector
    
    └── Koniec cewki do GND
        End of coil to GND
```

Parametry:
- Drut: 0.3mm emaliowany / enameled
- Zwoje: ~200 (eksperymentuj 150-250)
- Indukcyjność: ~2-5 mH
- Nawój równomiernie na środkowej części pręta

### Opcja 2: Gotowa antena AM / Ready AM Antenna
- Użyj anteny z radia AM/MW
- Częstotliwość rezonansowa ~500-1600kHz jest OK
- Zasięg może być większy

## Zasilanie / Power Supply

**Ważne!** XIAO ESP32C3 wymaga stabilnego 3.3V lub 5V (USB):

1. **USB (5V)** - najprostsze, przez port USB-C
2. **Bateria Li-Po 3.7V** - dla trybu przenośnego
   - Połącz do pinów BAT+ i BAT-
   - XIAO ma wbudowaną ładowarkę
3. **Zasilacz 5V** - przez pin 5V

### Pobór prądu / Power Consumption:
- Aktywny (WiFi ON): ~100-180mA
- Nadawanie (WiFi OFF): ~80-120mA  
- Deep Sleep: ~40-50μA
- Średnio: ~10-20mA (z deep sleep)

## Zasięg / Range

Oczekiwany zasięg / Expected range:
- **Podstawowy układ**: 3-7 metrów
- **Z lepszą anteną**: 5-15 metrów
- **Z wzmacniaczem MOSFET**: 10-30 metrów

Czynniki wpływające na zasięg / Range factors:
- Jakość anteny / antenna quality
- Orientacja anteny względem zegarka / antenna orientation to watch
- Metalowe przedmioty w pobliżu / metal objects nearby
- Siła sygnału (prąd kolektora) / signal strength

## Debugowanie / Debugging

### Test LED:
Możesz dodać LED do monitorowania sygnału:

```
GPIO4 ────[220Ω]────(LED)────GND
```

LED będzie migać zgodnie z sygnałem DCF77 (1 puls/sekundę).

## Uwagi bezpieczeństwa / Safety Notes

⚠️ **Ważne:**
- Nie przekraczaj 100mW mocy nadawczej (regulacje)
- Używaj tylko do celów osobistych (synchronizacja własnych zegarków)
- Nie zakłócaj innych urządzeń
- Zachowaj odstępy bezpieczne od anteny (>10cm)

## Lista zakupów na AliExpress / AliExpress Shopping List

Szukaj / Search for:
1. "XIAO ESP32C3" lub "Seeeduino XIAO ESP32C3"
2. "BC547 transistor NPN" lub "2N2222"
3. "Ferrite rod antenna" lub "AM antenna ferrite rod"
4. "Resistor kit 1/4W"
5. "Ceramic capacitor kit"
6. "0.3mm enameled copper wire"

Całkowity koszt: ~20-30 PLN (~5-8 USD)
