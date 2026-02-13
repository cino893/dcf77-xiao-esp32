# Build and Upload Guide / Przewodnik kompilacji i wgrywania

## 🔧 Przygotowanie środowiska / Environment Setup

### Krok 1: Zainstaluj Arduino IDE

#### Windows:
1. Pobierz z https://www.arduino.cc/en/software
2. Wybierz "Windows Win 10 and newer"
3. Zainstaluj (zalecane: Install for All Users)

#### macOS:
1. Pobierz z https://www.arduino.cc/en/software
2. Wybierz "macOS"
3. Przeciągnij do folderu Applications

#### Linux:
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install arduino

# Lub pobierz najnowszą wersję:
wget https://downloads.arduino.cc/arduino-ide/arduino-ide_latest_Linux_64bit.AppImage
chmod +x arduino-ide_*_Linux_64bit.AppImage
./arduino-ide_*_Linux_64bit.AppImage
```

### Krok 2: Dodaj obsługę ESP32

1. **Otwórz Arduino IDE**

2. **Dodaj URL do Board Manager**:
   - **Arduino IDE 2.x**: File → Preferences → Additional boards manager URLs
   - **Arduino IDE 1.x**: File → Preferences → Additional Boards Manager URLs
   
   Dodaj:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```

3. **Zainstaluj ESP32 Board Package**:
   - Tools → Board → Boards Manager
   - Wyszukaj: "esp32"
   - Znajdź "esp32 by Espressif Systems"
   - Kliknij "Install" (może potrwać kilka minut)

### Krok 3: Skonfiguruj XIAO ESP32C3

1. **Podłącz XIAO ESP32C3 do USB**
   - Użyj dobrego kabla USB-C (z transmisją danych)
   - Poczekaj na wykrycie urządzenia

2. **Wybierz płytkę**:
   - Tools → Board → ESP32 Arduino → **XIAO_ESP32C3**

3. **Wybierz port**:
   - Tools → Port → Wybierz odpowiedni port
   - **Windows**: COM3, COM4, itp.
   - **macOS**: /dev/cu.usbmodem*
   - **Linux**: /dev/ttyACM0, /dev/ttyUSB0

4. **Konfiguracja dodatkowych opcji**:
   - Tools → Upload Speed → **921600** (najszybsze)
   - Tools → USB CDC On Boot → **Enabled** (dla Serial Monitor)
   - Tools → CPU Frequency → **160MHz** (domyślne)
   - Tools → Flash Size → **4MB** (domyślne)
   - Tools → Partition Scheme → **Default 4MB**

## 📥 Pobieranie projektu / Downloading the Project

### Opcja A: Git Clone (zalecane)
```bash
git clone https://github.com/cino893/dcf77-xiao-esp32.git
cd dcf77-xiao-esp32
```

### Opcja B: Download ZIP
1. Idź do https://github.com/cino893/dcf77-xiao-esp32
2. Kliknij "Code" → "Download ZIP"
3. Rozpakuj do katalogu `dcf77-xiao-esp32`

## ⚙️ Konfiguracja / Configuration

### Krok 1: Kopiuj config.h
```bash
# W katalogu projektu:
cp config.h.example config.h
```

### Krok 2: Edytuj config.h
Otwórz `config.h` w edytorze tekstu i ustaw swoje dane WiFi:

```cpp
#define WIFI_SSID "TwojSSID"           // Nazwa sieci WiFi (2.4GHz!)
#define WIFI_PASSWORD "TwojeHaslo"     // Hasło do WiFi
```

**⚠️ WAŻNE:**
- WiFi MUSI być 2.4GHz (ESP32C3 nie obsługuje 5GHz)
- SSID i hasło są case-sensitive (wielkość liter ma znaczenie)
- Nie commituj config.h z prawdziwymi danymi do GitHuba

## 🔨 Kompilacja i wgrywanie / Compile and Upload

### Metoda 1: Arduino IDE (GUI)

1. **Otwórz projekt**:
   - File → Open
   - Wybierz `dcf77-xiao-esp32.ino`

2. **Sprawdź kod** (opcjonalnie):
   - Sketch → Verify/Compile (Ctrl+R / Cmd+R)
   - Poczekaj na komunikat "Done compiling"

3. **Wgraj kod**:
   - Sketch → Upload (Ctrl+U / Cmd+U)
   - Obserwuj postęp w dolnej części okna
   
   **Jeśli występuje błąd połączenia:**
   1. Przytrzymaj przycisk **BOOT** na XIAO
   2. Kliknij Upload
   3. Puść BOOT gdy zacznie się wgrywanie

4. **Otwórz Serial Monitor**:
   - Tools → Serial Monitor (Ctrl+Shift+M)
   - Ustaw baud rate: **115200**
   - Powinieneś zobaczyć logi startowe

### Metoda 2: Arduino CLI (zaawansowane)

#### Instalacja Arduino CLI:
```bash
# Linux/macOS
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh

# Windows (PowerShell)
iwr -useb https://raw.githubusercontent.com/arduino/arduino-cli/master/install.ps1 | iex
```

#### Konfiguracja:
```bash
# Inicjalizacja
arduino-cli config init

# Dodaj URL ESP32
arduino-cli config add board_manager.additional_urls https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

# Aktualizuj index
arduino-cli core update-index

# Zainstaluj ESP32
arduino-cli core install esp32:esp32
```

#### Kompilacja:
```bash
cd dcf77-xiao-esp32
arduino-cli compile --fqbn esp32:esp32:XIAO_ESP32C3 dcf77-xiao-esp32.ino
```

#### Wgrywanie:
```bash
# Sprawdź port
arduino-cli board list

# Wgraj (zastąp PORT odpowiednią wartością)
arduino-cli upload -p /dev/ttyACM0 --fqbn esp32:esp32:XIAO_ESP32C3 dcf77-xiao-esp32.ino
```

#### Monitor serial:
```bash
arduino-cli monitor -p /dev/ttyACM0 -c baudrate=115200
```

## 🧪 Testowanie / Testing

### Test 1: Podstawowy test GPIO
```bash
# Otwórz i wgraj:
examples/simple_gpio_test/simple_gpio_test.ino
```
**Oczekiwany wynik**: LED miga raz na sekundę

### Test 2: Test WiFi i NTP
```bash
# Edytuj WiFi credentials w pliku
# Następnie otwórz i wgraj:
examples/wifi_ntp_test/wifi_ntp_test.ino
```
**Oczekiwany wynik**: Połączenie z WiFi i wyświetlenie czasu

### Test 3: Pełny DCF77
```bash
# Wgraj główny sketch
dcf77-xiao-esp32.ino
```
**Oczekiwany wynik**: Zobacz "Logi oczekiwane" poniżej

## 📊 Logi oczekiwane / Expected Logs

### Prawidłowe uruchomienie:
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

### Błędy i rozwiązania:

#### "Failed to connect to WiFi"
```
Connecting to WiFi: YourSSID
..............................
Failed to connect to WiFi. Restarting...
```
**Rozwiązanie**: Sprawdź SSID, hasło i czy WiFi to 2.4GHz

#### "Failed to synchronize NTP time"
```
Synchronizing time with NTP server...
..........
Failed to synchronize NTP time!
```
**Rozwiązanie**: Sprawdź firewall, router, spróbuj innego serwera NTP

## 🐛 Rozwiązywanie problemów kompilacji / Build Troubleshooting

### Błąd: "Board not found"
```
Error: Board esp32:esp32:XIAO_ESP32C3 is unknown
```
**Rozwiązanie**: 
1. Sprawdź czy zainstalowałeś ESP32 board package
2. Zrestartuj Arduino IDE
3. Spróbuj ponownie zainstalować ESP32

### Błąd: "Port not found"
```
Error: port not found
```
**Rozwiązanie**:
1. Sprawdź czy kabel USB transmituje dane (nie tylko zasilanie)
2. Zainstaluj sterowniki USB-Serial (Windows)
3. Sprawdź Device Manager (Windows) lub dmesg (Linux)

### Błąd: "Timeout waiting for packet"
```
A fatal error occurred: Failed to connect to ESP32
```
**Rozwiązanie**:
1. **Metoda 1**: Przytrzymaj BOOT podczas uploadu
2. **Metoda 2**: Obniż Upload Speed (do 115200)
3. **Metoda 3**: Naciśnij RESET przed uploadem

### Błąd: "Not enough space"
```
Sketch uses 1450000 bytes (110%) of program storage
```
**Rozwiązanie**: Nie powinno się zdarzyć z tym projektem. Sprawdź czy:
1. Wybrałeś właściwą płytkę (XIAO_ESP32C3)
2. Flash Size ustawiony na 4MB

### Błąd: "config.h: No such file"
```
dcf77-xiao-esp32.ino:15:10: fatal error: config.h: No such file or directory
```
**Rozwiązanie**:
```bash
cp config.h.example config.h
# Następnie edytuj config.h
```

## 📝 Checklist przed wgraniem / Pre-Upload Checklist

- [ ] Arduino IDE zainstalowane z obsługą ESP32
- [ ] XIAO ESP32C3 podłączone przez USB
- [ ] Właściwy board wybrany (XIAO_ESP32C3)
- [ ] Właściwy port wybrany
- [ ] config.h istnieje i zawiera prawidłowe dane WiFi
- [ ] WiFi to 2.4GHz (nie 5GHz)
- [ ] Serial Monitor ustawiony na 115200 baud
- [ ] USB CDC On Boot włączone (Enabled)

## 🔄 Aktualizacje / Updates

### Aktualizacja kodu z Git:
```bash
cd dcf77-xiao-esp32
git pull origin main
```

### Aktualizacja ESP32 Core:
1. Tools → Board → Boards Manager
2. Znajdź "esp32"
3. Kliknij "Update" jeśli dostępne

## 💾 Backup konfiguracji / Configuration Backup

**WAŻNE**: Przed aktualizacją zrób kopię swojego `config.h`:
```bash
cp config.h config.h.backup
```

Po aktualizacji:
```bash
# Jeśli config.h został nadpisany:
cp config.h.backup config.h
```

## 🎯 Następne kroki / Next Steps

Po udanym wgraniu:
1. Przejdź do [HARDWARE.md](HARDWARE.md) - montaż obwodu
2. Przejdź do [README.md](README.md) - użytkowanie
3. W razie problemów: [TROUBLESHOOTING.md](TROUBLESHOOTING.md)

---

**Powodzenia z wgrywaniem! / Good luck with uploading!** 🚀
