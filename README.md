# NikolaChrono

**A DIY ESP32-powered smart desk clock featuring precise timekeeping, customizable alarms, live weather, a stopwatch, and wireless control through Bluetooth Low Energy.**

![Build Status](https://img.shields.io/badge/build-custom%20firmware-blue)
![Platform](https://img.shields.io/badge/platform-ESP32-orange)
![Language](https://img.shields.io/badge/language-C%2B%2B-00599C)
![License](https://img.shields.io/badge/license-MIT-green)
![Web App](https://img.shields.io/badge/web-GitHub%20Pages-222222)

## Overview

NikolaChrono is an open-source DIY smart desk clock built around the ESP32 DevKit and DS3231 real-time clock (RTC). It combines accurate timekeeping, a configurable daily alarm, snooze and stop controls, a stopwatch with lap timing, and live weather information from Open-Meteo without requiring an API key. A TM1637 seven-segment display shows the current time, while a 16x2 I2C LCD provides additional information through four interactive pages. Users can also configure the clock and control its features wirelessly using a Bluetooth Low Energy (BLE) web application hosted on GitHub Pages.

## Features

### Time & Alarm
- **Accurate timekeeping:** Uses the DS3231 RTC with a CR2032 backup battery to preserve time when main power is disconnected.
- **Daily alarm:** Configure an alarm time and enable or disable the alarm.
- **Repeat intervals:** Configure alarm repetition at intervals of 0, 5, 10, 15, 30, or 60 minutes.
- **Snooze:** Delay an active alarm by five minutes.
- **Stop:** Stop the active alarm immediately.
- **Stopwatch:** Measure elapsed time and record lap times.
- **Time synchronization:** Set the RTC time through the BLE command interface.

### Display
- **TM1637 display:** Continuously displays the current time in HH:MM format.
- **16x2 I2C LCD:** Provides four pages for alarm settings, weather, date, and stopwatch information.
- **Second indicator:** A 5 mm LED provides a visual indication of seconds.
- **Automatic weather preview:** Temporarily overrides the clock page with weather information every two minutes for five seconds.

### Weather
- **Live weather:** Retrieves current weather information from the Open-Meteo API.
- **No API key required:** Uses Open-Meteo's public weather API.
- **Background fetching:** Runs Wi-Fi and HTTP weather requests in a FreeRTOS task on ESP32 Core 1 to reduce interference with timekeeping and user interaction.
- **Automatic updates:** Refreshes weather data periodically without requiring manual requests.

### Connectivity
- **Bluetooth Low Energy:** Exposes a BLE service named `NikolaChrono`.
- **Nordic UART Service:** Uses the standard NUS service and characteristic UUIDs for BLE communication.
- **Web Bluetooth control:** Provides a browser-based interface built with HTML, CSS, and JavaScript, without a frontend framework.
- **Wi-Fi connectivity:** Connects to a configured wireless network for weather updates.
- **Automatic reconnection:** Attempts to reconnect to Wi-Fi when the connection is lost.
- **GitHub Pages:** Hosts the web application for convenient access from compatible browsers.

### UX
- **Five physical buttons:** MODE, PLUS, MINUS, SNOOZE, and STOP.
- **LCD navigation:** Cycle through the four information pages using MODE.
- **Stopwatch control:** Long-press MODE on the stopwatch page to start or stop the stopwatch.
- **Wireless configuration:** Read and update time and alarm settings through BLE commands.
- **Non-blocking design:** Uses FreeRTOS task scheduling for weather operations to keep the clock responsive.

## Hardware Requirements

| Component | Model | Interface | Pin(s) |
|---|---|---|---|
| Main microcontroller | ESP32 DevKit | GPIO / Wi-Fi / BLE | Board-dependent |
| Real-time clock | DS3231 | I2C | SDA: GPIO 21, SCL: GPIO 22 |
| Main time display | TM1637 4-digit 7-segment | GPIO | CLK: GPIO 18, DIO: GPIO 19 |
| Secondary display | 16x2 LCD with I2C module | I2C | SDA: GPIO 21, SCL: GPIO 22 |
| Mode button | Tactile push button | Digital input | GPIO 25 |
| Plus button | Tactile push button | Digital input | GPIO 26 |
| Minus button | Tactile push button | Digital input | GPIO 27 |
| Snooze button | Tactile push button | Digital input | GPIO 32 |
| Stop button | Tactile push button | Digital input | GPIO 33 |
| Alarm sound | Active buzzer | Digital output | GPIO 23 |
| Second indicator | 5 mm LED with current-limiting resistor | Digital output | GPIO 2 |
| RTC backup | CR2032 battery | RTC backup supply | DS3231 battery holder |
| Power supply | USB power source | USB | ESP32 USB connector |

**Important:** The pin assignments above are the proposed default wiring for this README. Confirm that they match the actual firmware before assembling the circuit. Check the ESP32 board's pin restrictions and the voltage requirements of every peripheral. Use a suitable current-limiting resistor for the LED and ensure the LCD, RTC, and TM1637 modules are electrically compatible with the ESP32's 3.3 V GPIO logic.

## Repository Structure

```text
NikolaChrono/
├── firmware/
│   └── NikolaChrono.ino
├── web/
│   └── index.html
├── docs/
│   └── wiring.md
├── README.md
├── LICENSE
└── .gitignore
