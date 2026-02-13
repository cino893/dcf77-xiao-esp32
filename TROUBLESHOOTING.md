# Troubleshooting Guide / Przewodnik rozwiązywania problemów

## 🔍 Diagnozowanie problemów / Problem Diagnosis

### 1. ❌ Urządzenie nie uruchamia się / Device won't start

**Symptomy / Symptoms:**
- Brak wyjścia na serial monitor
- LED nie miga
- Brak reakcji

**Rozwiązania / Solutions:**

#### A. Sprawdź zasilanie / Check power supply
```bash
# W serial monitor powinno być widoczne:
=== DCF77 Emulator for XIAO ESP32C3 ===
Boot count: 1
```

**Jeśli nie widać napisu / If you don't see the text:**
- ✅ Sprawdź kabel USB (użyj innego)
- ✅ Sprawdź port USB komputera
- ✅ Upewnij się, że wybrałeś właściwy port w Arduino IDE
- ✅ Zwiększ opóźnienie po otwarciu serial monitor (2-3 sekundy)

#### B. Problem z wgrywaniem kodu / Upload problem
```bash
# Błąd: "Failed to connect to ESP32"
```

**Rozwiązanie / Solution:**
1. Przytrzymaj przycisk BOOT na XIAO
2. Kliknij Upload w Arduino IDE
3. Poczekaj aż zacznie się upload
4. Puść przycisk BOOT

---

### 2. 📡 WiFi nie łączy się / WiFi won't connect

**Symptomy / Symptoms:**
```bash
Connecting to WiFi: YourSSID
............................
Failed to connect to WiFi. Restarting...
```

**Rozwiązania / Solutions:**

#### A. Sprawdź credentials w config.h
```cpp
// config.h - MUST match EXACTLY
#define WIFI_SSID "YourNetworkName"      // Case sensitive!
#define WIFI_PASSWORD "YourPassword123"   // Case sensitive!
```

**Typowe błędy / Common mistakes:**
- ❌ Spacje na początku/końcu SSID lub hasła
- ❌ Wielkość liter (case sensitive)
- ❌ Ukryte znaki (skopiuj-wklej może dodać)
- ❌ Hasło nieprawidłowe

#### B. Sprawdź typ sieci / Check network type
**XIAO ESP32C3 obsługuje TYLKO 2.4GHz!**

- ❌ Nie działa z: 5GHz, WiFi 6E
- ✅ Działa z: 2.4GHz (802.11 b/g/n)

**Jak sprawdzić / How to check:**
1. Na telefonie: Ustawienia → WiFi → Informacje o sieci
2. Na routerze: Panel administracyjny
3. Spróbuj z telefonem jako hotspot (2.4GHz)

#### C. Problemy z zasięgiem / Range issues
```cpp
// W setup(), dodaj logi:
Serial.print("WiFi RSSI: ");
Serial.println(WiFi.RSSI());
// RSSI > -70 dBm = dobry sygnał
// RSSI < -80 dBm = słaby, przenieś bliżej routera
```

---

### 3. ⏰ Czas się nie synchronizuje / Time won't sync

**Symptomy / Symptoms:**
```bash
Synchronizing time with NTP server...
..........
Failed to synchronize NTP time!
```

**Rozwiązania / Solutions:**

#### A. Sprawdź firewall / Check firewall
NTP używa portu UDP 123. Upewnij się że:
- ✅ Router nie blokuje NTP
- ✅ Firewall nie blokuje ESP32

#### B. Zmień serwer NTP / Change NTP server
```cpp
// W dcf77-xiao-esp32.ino, zmień:
#define NTP_SERVER "time.google.com"  // Google NTP
// lub / or
#define NTP_SERVER "time.cloudflare.com"  // Cloudflare
// lub lokalny / or local
#define NTP_SERVER "192.168.1.1"  // Twój router
```

#### C. Sprawdź strefę czasową / Check timezone
```cpp
// Upewnij się że strefa czasowa jest poprawna:
#define GMT_OFFSET_SEC 3600        // CET (UTC+1)
#define DAYLIGHT_OFFSET_SEC 3600   // +1h w lecie

// Dla Polski / For Poland: OK ✅
// Dla UK: 0, 3600
// Dla USA EST: -18000, 3600
```

---

### 4. 📻 Zegarek nie synchronizuje się / Watch won't sync

To najczęstszy problem! / Most common problem!

**Symptomy / Symptoms:**
- ESP32 działa, transmituje sygnał
- Zegarek nie wykrywa sygnału
- Synchronizacja kończy się błędem

**Rozwiązania krok po kroku / Step-by-step solutions:**

#### Krok 1: Sprawdź transmisję / Check transmission
W serial monitor powinno być widoczne:
```bash
=== Transmitting DCF77 signal for 03:25 ===
Encoded time: 03:26 13.02.2026 (Day 5)
M0100000100 0000011000 1000100001...
```

**Jeśli nie ma transmisji / If no transmission:**
- Czas może być niepoprawny
- Sprawdź minutę (musi być :00 sekundy)

#### Krok 2: Test LED / LED Test
Podłącz LED + rezystor 220Ω do GPIO4:
```
GPIO4 ─────[220Ω]─────(LED)─────GND
```

**LED powinno migać raz na sekundę / LED should blink once per second**
- Jeśli nie miga: Problem z kodem lub GPIO
- Jeśli miga: Hardware działa ✅

#### Krok 3: Sprawdź antenę / Check antenna

##### A. Polarity (polaryzacja)
```bash
# Sprawdź podłączenie:
Kolektor Q1 → Początek cewki anteny
Koniec cewki → GND
```

**Odwrotne połączenie = brak sygnału!**

##### B. Liczba zwojów / Number of turns
- Minimum: 150 zwojów
- Optymalne: 200 zwojów  
- Maksimum: 300 zwojów

**Jeśli za mało zwojów: Zasięg < 1m**

##### C. Jakość nawinięcia / Winding quality
- ❌ Zwoje luźne, nierównomierne
- ✅ Zwoje ciasne, równomierne, środek pręta

#### Krok 4: Pozycjonowanie zegarka / Watch positioning

**BARDZO WAŻNE! / VERY IMPORTANT!**

```
        [Antena ferrytowa - vertical]
              |
              |  10-30 cm
              ↓
        [Zegarek - płasko]
        
DOBRA orientacja ✅ / GOOD orientation:
- Zegarek PŁASKO na stole
- Antena PIONOWO lub POZIOMO (testuj oba)
- Odległość 10-30 cm

ZŁA orientacja ❌ / BAD orientation:
- Zegarek pod kątem
- Za daleko (>50cm)
- Za blisko (<5cm - nasycenie)
```

**Test orientacji / Orientation test:**
1. Połóż zegarek płasko
2. Trzymaj antenę pionowo nad zegarkiem (~20cm)
3. Powoli obracaj zegarek o 90° (4 pozycje)
4. Czekaj 15-30 sekund w każdej pozycji
5. Jedna z pozycji powinna zadziałać

#### Krok 5: Timing (czas transmisji) / Transmission timing

Casio synchronizuje się TYLKO w określonych godzinach:
- 🕑 2:00 - 3:00
- 🕒 3:00 - 4:00
- 🕓 4:00 - 5:00
- 🕔 5:00 - 6:00

**Poza tymi godzinami:**
- Niektóre zegarki WYMAGAJĄ manualnej synchronizacji
- Naciśnij i przytrzymaj przycisk ADJUST ~2 sekundy
- Zegarek powinien wyświetlić "RCV" lub podobne

#### Krok 6: Wzmocnienie sygnału / Signal amplification

**Jeśli nadal nie działa, zwiększ moc:**

##### Opcja A: Więcej zwojów
```bash
Zwiększ do 250-300 zwojów
Użyj dłuższego pręta (150-200mm)
```

##### Opcja B: MOSFET amplifier
```
Wymień BC547 na IRF540N:
- Gate → GPIO4 (przez 1kΩ)
- Source → GND
- Drain → Antena
```

##### Opcja C: Lepszy drut
```bash
Zamiast 0.3mm użyj 0.4-0.5mm
Mniejsza rezystancja = większy prąd = silniejsze pole
```

---

### 5. 🔋 Szybko rozładowuje baterię / Battery drains quickly

**Oczekiwane zużycie / Expected consumption:**
- Deep sleep: 40-50 μA
- Transmisja: 80-120 mA
- WiFi aktywne: 100-180 mA

**Jeśli bateria rozładowuje się w <24h:**

#### A. Deep sleep nie działa
```cpp
// Dodaj logi przed snem:
Serial.println("Going to sleep...");
Serial.flush();  // Ważne! Ensure data is sent
delay(100);
esp_deep_sleep_start();
```

#### B. WiFi nie wyłącza się
```cpp
// Po synchronizacji NTP, sprawdź:
WiFi.disconnect(true);
WiFi.mode(WIFI_OFF);
delay(100);
```

#### C. Zbyt częsta transmisja
```cpp
// Zmień harmonogram (tylko 2:00-3:00):
const int SYNC_HOURS[] = {2};
const int NUM_SYNC_HOURS = 1;
```

---

### 6. 🐛 Inne problemy / Other issues

#### A. Reset loop (ciągłe restartowanie)
```bash
=== DCF77 Emulator ===
Boot count: 15
Boot count: 16
Boot count: 17
...
```

**Przyczyny / Causes:**
- Watchdog timeout (kod trwa za długo)
- Problemy z pamięcią
- Uszkodzony kod

**Rozwiązanie / Solution:**
```bash
# Wymaż pamięć flash:
python -m esptool --port /dev/ttyUSB0 erase_flash

# Wgraj kod ponownie
```

#### B. Serial monitor pokazuje śmieci
```bash
ÿþý����������
```

**Rozwiązanie / Solution:**
1. Ustaw baud rate: **115200**
2. Sprawdź USB CDC On Boot: **Enabled**
3. Naciśnij reset na XIAO
4. Otwórz serial monitor ZARAZ po resecie

---

## 🧪 Testy diagnostyczne / Diagnostic Tests

### Test 1: Podstawowy / Basic Test
```cpp
// Prosty test GPIO:
void setup() {
  pinMode(4, OUTPUT);
}

void loop() {
  digitalWrite(4, HIGH);
  delay(500);
  digitalWrite(4, LOW);
  delay(500);
}
// LED powinno migać 1Hz
```

### Test 2: WiFi Test
```cpp
void setup() {
  Serial.begin(115200);
  WiFi.begin("SSID", "PASSWORD");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.println(WiFi.localIP());
}
```

### Test 3: NTP Test
```cpp
void setup() {
  // ... WiFi setup ...
  configTime(3600, 3600, "pool.ntp.org");
  
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  }
}
```

---

## 📞 Dalsze wsparcie / Further Support

Jeśli żaden z powyższych kroków nie pomógł:

1. **GitHub Issues**: https://github.com/cino893/dcf77-xiao-esp32/issues
2. **Dołącz**:
   - Logi z serial monitor (cały output)
   - Model zegarka Casio
   - Zdjęcie anteny i połączeń
   - Kod `config.h` (BEZ hasła WiFi!)
3. **Opisz szczegółowo**:
   - Co próbowałeś
   - Kiedy problem występuje
   - Jakie kroki wykonałeś

---

**Powodzenia! / Good luck!** 🍀
