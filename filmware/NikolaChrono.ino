#include <Wire.h>
#include <RTClib.h>
#include <TM1637Display.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <NimBLEDevice.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define TM_CLK 26
#define TM_DIO 25
#define BTN_MODE 17
#define BTN_PLUS 16
#define BTN_MINUS 4
#define BTN_SNOOZE 13
#define BTN_STOP 18
#define BUZZER_PIN 27
#define LED_SEC 32

const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";
const char* latitude = "-6.7924";
const char* longitude = "39.2083";

TM1637Display display(TM_CLK, TM_DIO);
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS3231 rtc;

int alarmHour = 7;
int alarmMinute = 0;
bool alarmEnabled = true;
int repeatInterval = 0;
int repeatOptions[] = {0, 5, 10, 15, 30, 60};
int repeatIndex = 0;

bool isRinging = false;
bool buzzerOn = false;
bool alarmFiredToday = false;
int lastAlarmDay = -1;
unsigned long ringStart = 0;
unsigned long snoozeUntil = 0;
bool snoozeActive = false;
unsigned long lastRepeatMs = 0;
unsigned long alarmTriggerMs = 0;

int lcdPage = 0;
bool weatherOverride = false;
unsigned long weatherOverrideStart = 0;
unsigned long lastWeatherShow = 0;
const unsigned long WEATHER_SHOW_INTERVAL = 120000UL;
const unsigned long WEATHER_SHOW_DURATION = 5000UL;

float weatherTemp = 0.0;
int weatherCode = -1;
int weatherHumidity = 0;
bool weatherValid = false;
const unsigned long WEATHER_INTERVAL = 900000UL;
unsigned long lastWifiCheck = 0;

bool stopwatchRunning = false;
unsigned long stopwatchStartMs = 0;
unsigned long stopwatchAccumMs = 0;
unsigned long stopwatchLapMs = 0;
bool stopwatchLapValid = false;

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
NimBLECharacteristic* pTxCharacteristic = nullptr;
NimBLEServer* pServer = nullptr;

const char* getWeatherText(int code);
const char* getDayName(int dow);
const char* getDayShort(int dow);
void handleCommand(String cmd);
void fetchWeather();

class RxCallbacks : public NimBLECharacteristicCallbacks {
  void onWrite(NimBLECharacteristic* pChar) override {
#if defined(NIMBLE_ARDUINO_VERSION_MAJOR) && (NIMBLE_ARDUINO_VERSION_MAJOR >= 2)
    NimBLEAttValue val = pChar->getValue();
    String cmd = String((const char*)val.data(), val.length());
#else
    std::string val = pChar->getValue();
    String cmd = String(val.c_str());
#endif
    cmd.trim();
    if (cmd.length() > 0) handleCommand(cmd);
  }
};

void handleCommand(String cmd) {
  String reply = "OK";

  if (cmd == "TIME?") {
    DateTime n = rtc.now();
    char buf[16];
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d", n.hour(), n.minute(), n.second());
    reply = buf;
  }
  else if (cmd.startsWith("SET TIME ")) {
    String t = cmd.substring(9);
    int c1 = t.indexOf(':');
    int c2 = t.indexOf(':', c1 + 1);
    if (c1 > 0 && c2 > c1) {
      int h = t.substring(0, c1).toInt();
      int m = t.substring(c1 + 1, c2).toInt();
      int s = t.substring(c2 + 1).toInt();
      if (h >= 0 && h < 24 && m >= 0 && m < 60 && s >= 0 && s < 60) {
        DateTime cur = rtc.now();
        rtc.adjust(DateTime(cur.year(), cur.month(), cur.day(), h, m, s));
        reply = "Time set";
      } else {
        reply = "Error: invalid time";
      }
    } else {
      reply = "Error: use SET TIME HH:MM:SS";
    }
  }
  else if (cmd == "ALARM?") {
    char buf[48];
    if (repeatInterval == 0) {
      snprintf(buf, sizeof(buf), "%02d:%02d %s ONCE", alarmHour, alarmMinute, alarmEnabled ? "ON" : "OFF");
    } else {
      snprintf(buf, sizeof(buf), "%02d:%02d %s REPEAT %dM", alarmHour, alarmMinute, alarmEnabled ? "ON" : "OFF", repeatInterval);
    }
    reply = buf;
  }
  else if (cmd.startsWith("SET ALARM ")) {
    String v = cmd.substring(10);
    if (v == "OFF") {
      alarmEnabled = false;
      reply = "Alarm disabled";
    } else if (v == "ON") {
      alarmEnabled = true;
      reply = "Alarm enabled";
    } else {
      int c = v.indexOf(':');
      if (c > 0) {
        int h = v.substring(0, c).toInt();
        int m = v.substring(c + 1).toInt();
        if (h >= 0 && h < 24 && m >= 0 && m < 60) {
          alarmHour = h;
          alarmMinute = m;
          alarmEnabled = true;
          reply = "Alarm set";
        } else {
          reply = "Error: invalid time";
        }
      } else {
        reply = "Error: use SET ALARM HH:MM";
      }
    }
  }
  else if (cmd.startsWith("SET REPEAT ")) {
    int r = cmd.substring(11).toInt();
    bool found = false;
    for (int i = 0; i < 6; i++) {
      if (repeatOptions[i] == r) {
        repeatIndex = i;
        repeatInterval = r;
        found = true;
        break;
      }
    }
    if (found) {
      if (r == 0) reply = "Repeat: once";
      else reply = String("Repeat: ") + r + " minutes";
    } else {
      reply = "Error: choose 0,5,10,15,30,60";
    }
  }
  else if (cmd == "SNOOZE") {
    if (isRinging) {
      isRinging = false;
      buzzerOn = false;
      noTone(BUZZER_PIN);
      snoozeUntil = millis() + 300000UL;
      snoozeActive = true;
      reply = "Snoozed 5 minutes";
    } else {
      reply = "Alarm not ringing";
    }
  }
  else if (cmd == "STOP") {
    isRinging = false;
    buzzerOn = false;
    noTone(BUZZER_PIN);
    snoozeActive = false;
    reply = "Alarm stopped";
  }
  else if (cmd == "WEATHER?") {
    if (weatherValid) {
      char buf[40];
      snprintf(buf, sizeof(buf), "%.1fC %d%% %.8s", weatherTemp, weatherHumidity, getWeatherText(weatherCode));
      reply = buf;
    } else {
      reply = "Weather not available";
    }
  }
  else {
    reply = "Unknown command";
  }

  if (pTxCharacteristic && pServer && pServer->getConnectedCount() > 0) {
    pTxCharacteristic->setValue((uint8_t*)reply.c_str(), reply.length());
    pTxCharacteristic->notify();
  }
}

const char* getWeatherText(int code) {
  if (code == 0) return "Sunny";
  if (code >= 1 && code <= 3) return "Cloudy";
  if (code == 45 || code == 48) return "Fog";
  if (code >= 51 && code <= 57) return "Drizzle";
  if (code >= 61 && code <= 67) return "Rain";
  if (code >= 71 && code <= 77) return "Snow";
  if (code >= 80 && code <= 82) return "HvyRain";
  if (code == 85 || code == 86) return "HvySnow";
  if (code >= 95) return "T-Storm";
  return "?";
}

const char* getDayName(int dow) {
  static const char* days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
  if (dow < 0 || dow > 6) return "?";
  return days[dow];
}

const char* getDayShort(int dow) {
  static const char* days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  if (dow < 0 || dow > 6) return "?";
  return days[dow];
}

void fetchWeather() {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient http;
  char url[256];
  snprintf(url, sizeof(url),
    "https://api.open-meteo.com/v1/forecast?latitude=%s&longitude=%s"
    "&current=temperature_2m,relative_humidity_2m,weather_code&timezone=auto",
    latitude, longitude);

  http.begin(url);
  http.setConnectTimeout(3000);
  http.setTimeout(5000);

  int code = http.GET();
  if (code == 200) {
    String payload = http.getString();
    JsonDocument doc;
    if (!deserializeJson(doc, payload)) {
      weatherTemp = doc["current"]["temperature_2m"];
      weatherHumidity = doc["current"]["relative_humidity_2m"];
      weatherCode = doc["current"]["weather_code"];
      weatherValid = true;
    }
  }
  http.end();
}

unsigned long lastPressMode = 0;
unsigned long lastPressPlus = 0;
unsigned long lastPressMinus = 0;
unsigned long lastPressSnooze = 0;
unsigned long lastPressStop = 0;
unsigned long modePressStart = 0;
bool modeLongPressed = false;
const unsigned long DEBOUNCE_MS = 250;
const unsigned long LONG_PRESS_MS = 800;

void checkButtons() {
  unsigned long now = millis();

  if (digitalRead(BTN_MODE) == LOW) {
    if (modePressStart == 0) modePressStart = now;
    if (!modeLongPressed && lcdPage == 3 && now - modePressStart > LONG_PRESS_MS) {
      modeLongPressed = true;
      stopwatchRunning = !stopwatchRunning;
      if (stopwatchRunning) {
        stopwatchStartMs = now;
      } else {
        stopwatchAccumMs += now - stopwatchStartMs;
      }
      lastPressMode = now;
    }
  } else {
    if (modePressStart != 0 && !modeLongPressed && now - lastPressMode > DEBOUNCE_MS) {
      lcdPage = (lcdPage + 1) % 4;
      lastPressMode = now;
    }
    modePressStart = 0;
    modeLongPressed = false;
  }

  if (digitalRead(BTN_PLUS) == LOW && now - lastPressPlus > DEBOUNCE_MS) {
    lastPressPlus = now;
    if (lcdPage == 3 && stopwatchRunning) {
      stopwatchLapMs = stopwatchAccumMs + (now - stopwatchStartMs);
      stopwatchLapValid = true;
    }
  }

  if (digitalRead(BTN_MINUS) == LOW && now - lastPressMinus > DEBOUNCE_MS) {
    lastPressMinus = now;
    if (isRinging) {
      alarmEnabled = !alarmEnabled;
    } else if (lcdPage == 3 && !stopwatchRunning) {
      stopwatchAccumMs = 0;
      stopwatchLapMs = 0;
      stopwatchLapValid = false;
    }
  }

  if (digitalRead(BTN_SNOOZE) == LOW && now - lastPressSnooze > DEBOUNCE_MS) {
    lastPressSnooze = now;
    if (isRinging) {
      isRinging = false;
      buzzerOn = false;
      noTone(BUZZER_PIN);
      snoozeUntil = now + 300000UL;
      snoozeActive = true;
    }
  }

  if (digitalRead(BTN_STOP) == LOW && now - lastPressStop > DEBOUNCE_MS) {
    lastPressStop = now;
    if (isRinging) {
      isRinging = false;
      buzzerOn = false;
      noTone(BUZZER_PIN);
      snoozeActive = false;
    }
  }
}

char lcdLine0[17], lcdLine1[17];
unsigned long lastLcdUpdate = 0;

void updateLCD() {
  if (millis() - lastLcdUpdate < 400) return;
  lastLcdUpdate = millis();

  DateTime now = rtc.now();
  char line0[17], line1[17];

  if (!weatherOverride && lcdPage == 0 && millis() - lastWeatherShow > WEATHER_SHOW_INTERVAL) {
    weatherOverride = true;
    weatherOverrideStart = millis();
    lastWeatherShow = millis();
  }
  if (weatherOverride && millis() - weatherOverrideStart > WEATHER_SHOW_DURATION) {
    weatherOverride = false;
  }

  int effectivePage = weatherOverride ? 1 : lcdPage;

  switch (effectivePage) {
    case 0:
      snprintf(line0, sizeof(line0), "Alarm %s %02d:%02d  ",
               alarmEnabled ? "ON " : "OFF", alarmHour, alarmMinute);
      snprintf(line1, sizeof(line1), "%s %02d/%02d  ",
               getDayShort(now.dayOfTheWeek()), now.day(), now.month());
      break;
    case 1:
      if (weatherValid) {
        snprintf(line0, sizeof(line0), "Temp:%.1fC  ", weatherTemp);
        snprintf(line1, sizeof(line1), "%.8s Hum:%d%%", getWeatherText(weatherCode), weatherHumidity);
      } else if (WiFi.status() == WL_CONNECTED) {
        snprintf(line0, sizeof(line0), "Weather      ");
        snprintf(line1, sizeof(line1), "Loading...   ");
      } else {
        snprintf(line0, sizeof(line0), "WiFi: OFFLINE ");
        snprintf(line1, sizeof(line1), "Check network");
      }
      break;
    case 2:
      snprintf(line0, sizeof(line0), "%s  ", getDayName(now.dayOfTheWeek()));
      snprintf(line1, sizeof(line1), "%02d/%02d/%04d  ", now.day(), now.month(), now.year());
      break;
    case 3: {
      unsigned long elapsed = stopwatchAccumMs;
      if (stopwatchRunning) elapsed += millis() - stopwatchStartMs;
      unsigned long totalSec = elapsed / 1000;
      unsigned long mins = totalSec / 60;
      unsigned long secs = totalSec % 60;
      unsigned long centi = (elapsed % 1000) / 10;
      if (mins > 99) { mins = 99; secs = 59; centi = 99; }
      snprintf(line0, sizeof(line0), "SW %02lu:%02lu.%02lu ",
               mins, secs, centi);
      if (stopwatchLapValid) {
        unsigned long lapSec = stopwatchLapMs / 1000;
        snprintf(line1, sizeof(line1), "Lap %02lu:%02lu   ",
                 lapSec / 60, lapSec % 60);
      } else {
        snprintf(line1, sizeof(line1), "%s  ", stopwatchRunning ? "RUNNING" : "STOPPED");
      }
      break;
    }
  }

  if (strncmp(lcdLine0, line0, 16) != 0) {
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 0);
    lcd.print(line0);
    strcpy(lcdLine0, line0);
  }
  if (strncmp(lcdLine1, line1, 16) != 0) {
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print(line1);
    strcpy(lcdLine1, line1);
  }
}

void weatherTask(void* pv) {
  vTaskDelay(pdMS_TO_TICKS(5000));
  for (;;) {
    if (WiFi.status() != WL_CONNECTED) {
      vTaskDelay(pdMS_TO_TICKS(30000));
      continue;
    }
    fetchWeather();
    vTaskDelay(pdMS_TO_TICKS(WEATHER_INTERVAL));
  }
}

void setup() {
  Serial.begin(115200);
  delay(200);

  pinMode(BTN_MODE, INPUT_PULLUP);
  pinMode(BTN_PLUS, INPUT_PULLUP);
  pinMode(BTN_MINUS, INPUT_PULLUP);
  pinMode(BTN_SNOOZE, INPUT_PULLUP);
  pinMode(BTN_STOP, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_SEC, OUTPUT);

  Wire.begin(21, 22);

  if (!rtc.begin()) Serial.println("RTC not found");
  if (rtc.lostPower()) rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  display.setBrightness(7);
  display.clear();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("NikolaChrono");

  WiFi.begin(ssid, password);
  int t = 0;
  while (WiFi.status() != WL_CONNECTED && t++ < 30) delay(500);
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi IP: ");
    Serial.println(WiFi.localIP());
  }

  NimBLEDevice::init("NikolaChrono");
  NimBLEDevice::setMTU(185);
  pServer = NimBLEDevice::createServer();
  NimBLEService* svc = pServer->createService(SERVICE_UUID);

  NimBLECharacteristic* rx = svc->createCharacteristic(
    CHARACTERISTIC_UUID_RX, NIMBLE_PROPERTY::WRITE);
  rx->setCallbacks(new RxCallbacks());

  pTxCharacteristic = svc->createCharacteristic(
    CHARACTERISTIC_UUID_TX, NIMBLE_PROPERTY::NOTIFY);

  svc->start();
  NimBLEDevice::getAdvertising()->addServiceUUID(SERVICE_UUID);
  NimBLEDevice::getAdvertising()->start();
  Serial.println("BLE NikolaChrono");

  lcd.clear();
  strcpy(lcdLine0, "                ");
  strcpy(lcdLine1, "                ");

  xTaskCreatePinnedToCore(weatherTask, "weather", 8192, NULL, 1, NULL, 1);
}

void loop() {
  DateTime now = rtc.now();

  display.showNumberDecEx(now.hour() * 100 + now.minute(), 0b01000000, true);
  digitalWrite(LED_SEC, (now.second() % 2) ? HIGH : LOW);

  checkButtons();
  updateLCD();

  if (WiFi.status() != WL_CONNECTED && millis() - lastWifiCheck > 30000UL) {
    lastWifiCheck = millis();
    WiFi.disconnect();
    WiFi.begin(ssid, password);
  }

  if (now.day() != lastAlarmDay) {
    lastAlarmDay = now.day();
    alarmFiredToday = false;
    snoozeActive = false;
  }

  if (alarmEnabled && !alarmFiredToday && !snoozeActive
      && now.hour() == alarmHour && now.minute() == alarmMinute) {
    isRinging = true;
    alarmFiredToday = true;
    ringStart = millis();
    alarmTriggerMs = millis();
    lastRepeatMs = millis();
  }

  if (snoozeActive && millis() >= snoozeUntil) {
    isRinging = true;
    snoozeActive = false;
    ringStart = millis();
  }

  if (alarmEnabled && repeatInterval > 0 && !isRinging && !snoozeActive
      && alarmFiredToday && (millis() - lastRepeatMs > repeatInterval * 60000UL)
      && (millis() - alarmTriggerMs < 3600000UL)) {
    lastRepeatMs = millis();
    isRinging = true;
    ringStart = millis();
  }

  if (isRinging && millis() - ringStart > 60000UL) {
    isRinging = false;
  }

  if (isRinging) {
    bool beepPhase = ((millis() - ringStart) % 1000) < 500;
    if (beepPhase && !buzzerOn) {
      tone(BUZZER_PIN, 1000);
      buzzerOn = true;
    }
    if (!beepPhase && buzzerOn) {
      noTone(BUZZER_PIN);
      buzzerOn = false;
    }
  } else if (buzzerOn) {
    noTone(BUZZER_PIN);
    buzzerOn = false;
  }
}
