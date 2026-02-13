# DCF77 Emulator dla Xiao ESP32 C3 / DCF77 Emulator for Xiao ESP32 C3

[🇵🇱 Polski](#polski) | [🇬🇧 English](#english)

---

## 🇵🇱 Polski

### Opis projektu

Emulator sygnału DCF77 dla zegarków Casio Multiband i innych odbiorników czasu radiowego. Urządzenie oparte na **Seeed Studio XIAO ESP32C3** synchronizuje czas przez WiFi (NTP), a następnie emituje sygnał DCF77, który może być odbierany przez zegarki w promieniu 5-15 metrów.

### ✨ Główne funkcje

- 🕐 **Automatyczna synchronizacja**: Wybudza się w godzinach synchronizacji zegarków Casio (2-5 rano)
- 📡 **Emulacja DCF77**: Pełna implementacja protokołu DCF77 z kodowaniem BCD
- 🌐 **Synchronizacja NTP**: Pobiera dokładny czas z serwerów NTP przez WiFi
- 🔋 **Oszczędzanie energii**: Deep sleep pomiędzy transmisjiami (pobór ~40-50μA)
- 💰 **Tanie komponenty**: Całkowity koszt ~20-30 PLN (części z AliExpress)
- 📏 **Zasięg 5-15m**: Wystarczający dla domu/mieszkania

### 🛠️ Wymagane komponenty

1. **Seeed Studio XIAO ESP32C3** (~15 PLN)
2. **Tranzystor NPN** (BC547 lub 2N2222) (~0.10 PLN)
3. **Rezystory**: 1kΩ i 10kΩ (~0.10 PLN)
4. **Kondensator 100nF** (~0.10 PLN)
5. **Antena ferrytowa** z nawiniętym drutem (~5-20 PLN)
   - Pręt ferrytowy 10mm × 100mm
   - ~200 zwojów drutu emaliowanego 0.3mm

**Całkowity koszt**: ~20-30 PLN

Szczegółowy schemat i instrukcje montażu: zobacz [HARDWARE.md](HARDWARE.md)

### 📦 Instalacja oprogramowania

#### Wymagania wstępne

1. **Arduino IDE** (wersja 1.8.x lub 2.x)
2. **Obsługa ESP32** w Arduino IDE:
   - File → Preferences → Additional Board Manager URLs
   - Dodaj: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
   - Tools → Board → Boards Manager → Szukaj "ESP32" → Zainstaluj

#### Konfiguracja

1. **Sklonuj repozytorium**:
```bash
git clone https://github.com/cino893/dcf77-xiao-esp32.git
cd dcf77-xiao-esp32
```

2. **Skonfiguruj WiFi**:
```bash
cp config.h.example config.h
```

3. **Edytuj `config.h`** i ustaw swoje dane WiFi:
```cpp
#define WIFI_SSID "TwojSSID"
#define WIFI_PASSWORD "TwojeHaslo"
```

4. **Otwórz projekt w Arduino IDE**:
   - Otwórz plik `dcf77-xiao-esp32.ino`

5. **Wybierz płytkę**:
   - Tools → Board → ESP32 Arduino → XIAO_ESP32C3

6. **Skonfiguruj opcje**:
   - Tools → Upload Speed → 921600
   - Tools → USB CDC On Boot → Enabled

7. **Wgraj kod**:
   - Sketch → Upload

### 🔌 Podłączanie sprzętu

```
XIAO ESP32C3 Pin D2 (GPIO4) → Rezystor 1kΩ → Baza tranzystora BC547
Emiter tranzystora → GND
Kolektor tranzystora → Antena (początek cewki)
Antena (koniec cewki) → GND
Pin 10kΩ między bazą a GND (opcjonalnie)
```

Pełny schemat: zobacz [HARDWARE.md](HARDWARE.md)

### 🚀 Użytkowanie

1. **Podłącz zasilanie** (USB lub bateria Li-Po 3.7V)
2. **Urządzenie automatycznie**:
   - Połączy się z WiFi
   - Zsynchronizuje czas z NTP
   - Rozpocznie transmisję sygnału DCF77
3. **Ustaw zegarek Casio** w tryb ręcznej synchronizacji:
   - Umieść zegarek w odległości 10-50cm od anteny
   - Wyłącz inne źródła zakłóceń (telefony, komputery)
   - Rozpocznij synchronizację na zegarku (patrz instrukcja zegarka)
4. **Obserwuj serial monitor** (115200 baud) - zobaczysz transmitowane bity

### ⏰ Harmonogram pracy

Urządzenie automatycznie wybudza się w godzinach synchronizacji Casio:
- **2:00** - 3:00
- **3:00** - 4:00  
- **4:00** - 5:00
- **5:00** - 6:00

Pomiędzy sesjami przechodzi w tryb deep sleep (~40-50μA).

### 🔧 Rozwiązywanie problemów

**Zegarek nie synchronizuje się:**
- Sprawdź jakość połączeń
- Upewnij się, że antena jest poprawnie nawinięta
- Zmniejsz odległość (10-30cm)
- Sprawdź orientację anteny (obrót o 90°)
- Zweryfikuj czas w serial monitor

**WiFi nie łączy się:**
- Sprawdź SSID i hasło w `config.h`
- Upewnij się, że sieć WiFi to 2.4GHz (nie 5GHz)
- Sprawdź zasięg WiFi

**Słaby zasięg:**
- Zwiększ liczbę zwojów anteny (do 250)
- Użyj dłuższego pręta ferrytowego
- Dodaj wzmacniacz MOSFET (zobacz HARDWARE.md)

### 📊 Specyfikacja techniczna

- **Mikrokontroler**: ESP32-C3 (RISC-V, 160MHz)
- **Protokół**: DCF77 (amplitude modulation)
- **Częstotliwość**: 77.5 kHz (emulacja)
- **Format danych**: BCD (Binary Coded Decimal)
- **Synchronizacja**: NTP (Network Time Protocol)
- **Strefa czasowa**: CET/CEST (UTC+1/+2)
- **Pobór prądu**: 40-50μA (sleep) / 80-120mA (transmisja)
- **Zasięg**: 5-15 metrów (zależny od anteny)

### 📝 Licencja

MIT License - zobacz [LICENSE](LICENSE)

### 🤝 Współpraca

Pull requesty są mile widziane! Dla większych zmian, proszę najpierw otwórz issue.

### 📚 Źródła

- [DCF77 Protocol Specification](https://en.wikipedia.org/wiki/DCF77)
- [XIAO ESP32C3 Documentation](https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/)
- [ESP32 Deep Sleep Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-reference/system/sleep_modes.html)

---

## 🇬🇧 English

### Project Description

DCF77 signal emulator for Casio Multiband watches and other radio-controlled clocks. Device based on **Seeed Studio XIAO ESP32C3** synchronizes time via WiFi (NTP), then emits DCF77 signal that can be received by watches within 5-15 meters range.

### ✨ Key Features

- 🕐 **Automatic synchronization**: Wakes up during Casio watch sync hours (2-5 AM)
- 📡 **DCF77 emulation**: Full DCF77 protocol implementation with BCD encoding
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
```

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

- [DCF77 Protocol Specification](https://en.wikipedia.org/wiki/DCF77)
- [XIAO ESP32C3 Documentation](https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/)
- [ESP32 Deep Sleep Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-reference/system/sleep_modes.html)