# Quick Reference / Szybki przewodnik

## ⚡ Szybki start / Quick Start

### 5-minutowa instalacja / 5-minute setup:

1. **Sklonuj repo**:
   ```bash
   git clone https://github.com/cino893/dcf77-xiao-esp32.git
   ```

2. **Skonfiguruj WiFi**:
   ```bash
   cd dcf77-xiao-esp32
   cp config.h.example config.h
   nano config.h  # Edytuj SSID i hasło
   ```

3. **Wgraj kod**:
   - Otwórz `dcf77-xiao-esp32.ino` w Arduino IDE
   - Tools → Board → XIAO_ESP32C3
   - Tools → Port → [wybierz port]
   - Upload (Ctrl+U)

4. **Zbuduj hardware**:
   - Zobacz schemat w [HARDWARE.md](HARDWARE.md)
   - Połącz: GPIO4 → 1kΩ → BC547 (baza) → antena

5. **Testuj**:
   - Połóż zegarek Casio ~20cm od anteny
   - Ustaw na ręczną synchronizację
   - Czekaj ~1 minutę

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

## 🔌 Połączenia / Connections

### Minimalna konfiguracja:
```
XIAO D2 (GPIO4) ──[1kΩ]── BC547 (B)
                           BC547 (E) ── GND
                           BC547 (C) ── Antena ── GND
```

### Pełna konfiguracja z filtrowaniem:
```
XIAO 3V3 ──[100nF]── GND
XIAO D2 ──[1kΩ]── BC547 (B)
          [10kΩ]── GND (opcjonalnie)
BC547 (E) ── GND
BC547 (C) ── Antena ── GND
```

## 🕐 Godziny synchronizacji / Sync Hours

Urządzenie wybudza się automatycznie:
- **02:00** - transmisja 1h
- **03:00** - transmisja 1h
- **04:00** - transmisja 1h
- **05:00** - transmisja 1h

Ręczne wymuszenie: Przytrzymaj przycisk ADJUST na zegarku

## 📡 Parametry DCF77

| Parameter | Value |
|-----------|-------|
| Częstotliwość / Frequency | 77.5 kHz (emulowane) |
| Modulacja / Modulation | Amplituda / AM |
| Bit "0" | 100ms redukcji |
| Bit "1" | 200ms redukcji |
| Marker minuty | 1000ms bez sygnału |
| Zakres / Range | 5-15 metrów |

## 🔋 Pobór prądu / Power Consumption

| Stan / State | Prąd / Current |
|--------------|----------------|
| Deep Sleep | 40-50 μA |
| WiFi aktywne | 100-180 mA |
| Transmisja | 80-120 mA |
| **Średnio** | **~10-20 mA** |

### Czas pracy na baterii / Battery Life:
- **500mAh**: ~25-50 godzin (1-2 dni)
- **1000mAh**: ~50-100 godzin (2-4 dni)
- **2000mAh**: ~100-200 godzin (4-8 dni)

## 🛠️ Komponenty / Parts List

### Kup na AliExpress (całość ~20-30 PLN):

| Komponent | Ilość | Cena | Szukaj |
|-----------|-------|------|--------|
| XIAO ESP32C3 | 1 | ~15 PLN | "Seeeduino XIAO ESP32C3" |
| BC547 NPN | 1-10 | ~1 PLN | "BC547 transistor" |
| Rezystor 1kΩ | 1 | ~0.10 PLN | "1/4W resistor kit" |
| Rezystor 10kΩ | 1 | ~0.10 PLN | "1/4W resistor kit" |
| Kondensator 100nF | 1 | ~0.10 PLN | "ceramic capacitor" |
| Pręt ferrytowy | 1 | ~5-10 PLN | "ferrite rod 10mm" |
| Drut 0.3mm | 10m | ~5 PLN | "enameled copper wire" |

**Całość**: ~25 PLN (~6 USD)

## 🧪 Szybkie testy / Quick Tests

### Test 1: LED Blink
```cpp
// Upload: examples/simple_gpio_test/
// Oczekiwane: LED miga 1 Hz
```

### Test 2: WiFi
```cpp
// Upload: examples/wifi_ntp_test/
// Oczekiwane: "WiFi connected! 192.168.x.x"
```

### Test 3: Antena
```
1. Podłącz LED do GPIO4
2. Wgraj główny kod
3. LED miga raz/sekundę = działa ✅
```

## 📏 Wymiary anteny / Antenna Dimensions

### Standardowa:
```
Pręt ferrytowy: 10mm (Ø) × 100mm (L)
Zwoje: 200
Drut: 0.3mm enameled
Zasięg: 5-10m
```

### Zwiększony zasięg:
```
Pręt ferrytowy: 10mm (Ø) × 200mm (L)
Zwoje: 300
Drut: 0.4-0.5mm enameled
Zasięg: 10-20m
```

### Z wzmacniaczem MOSFET:
```
Użyj IRF540N zamiast BC547
+ wzmocnienie ~10x
Zasięg: 20-30m
```

## 🎯 Pozycjonowanie zegarka / Watch Positioning

```
     [Antenna]
         │
         │ 10-30 cm
         ↓
    ┌─────────┐
    │ Casio   │  ← Płasko / Flat
    │  Watch  │
    └─────────┘
    
✅ DOBRZE / GOOD:
- Płasko na stole
- 10-30 cm od anteny
- Obrót 0°, 90°, 180°, 270° (testuj)

❌ ŹLE / BAD:
- Pionowo (na boku)
- Za daleko (>50cm)
- Za blisko (<5cm)
- Pod kątem
```

## 📞 Pomoc / Support

| Problem | Zobacz / See |
|---------|--------------|
| Nie kompiluje się | [BUILD.md](BUILD.md) |
| Problemy z hardware | [HARDWARE.md](HARDWARE.md) |
| Zegarek nie sync | [TROUBLESHOOTING.md](TROUBLESHOOTING.md) |
| Zaawansowane | [ADVANCED.md](ADVANCED.md) |
| GitHub Issues | https://github.com/cino893/dcf77-xiao-esp32/issues |

## 🔑 Kluczowe pliki / Key Files

```
dcf77-xiao-esp32/
├── dcf77-xiao-esp32.ino    ← Główny kod / Main code
├── config.h                 ← Twoje WiFi / Your WiFi (nie commituj!)
├── config.h.example         ← Template
├── README.md                ← Główna dokumentacja
├── BUILD.md                 ← Jak wgrać kod
├── HARDWARE.md              ← Schemat i montaż
├── TROUBLESHOOTING.md       ← Rozwiązywanie problemów
├── ADVANCED.md              ← Zaawansowane funkcje
└── examples/
    ├── simple_gpio_test/    ← Test GPIO
    └── wifi_ntp_test/       ← Test WiFi/NTP
```

## 🎨 Status LED (opcjonalny)

Dodaj LED do GPIO4 dla wizualizacji:
```
GPIO4 ──[220Ω]──(LED)──GND
```

Znaczenie migania:
- **1 puls/sekundę**: Transmisja DCF77 OK ✅
- **Brak pulsów**: Nie transmituje (sprawdź czas)
- **Ciągłe świecenie**: Problem z kodem

## 🔄 Aktualizacje OTA (przyszłość)

Planowane w następnych wersjach:
- Web interface (192.168.x.x)
- WiFi AP mode dla konfiguracji
- OTA updates (bez USB)
- OLED display
- Statystyki synchronizacji

## 📚 Linki / Links

- **GitHub Repo**: https://github.com/cino893/dcf77-xiao-esp32
- **XIAO Wiki**: https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/
- **DCF77 Info**: https://en.wikipedia.org/wiki/DCF77
- **ESP32 Docs**: https://docs.espressif.com/

---

**Miłego hackowania! / Happy hacking!** 🎉
