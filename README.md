# Smart Home IoT Project

## Overview

This project is a complete end-to-end **Smart Home IoT system** that integrates an ESP32 microcontroller with MQTT communication, Node-RED flow processing, Firebase Firestore cloud database, and a Flutter-based mobile application.

The goal is to create a seamless environment where sensor data (temperature, humidity, light) from the ESP32 is sent via MQTT to a cloud broker, routed through Node-RED into Firestore, and then displayed live in a mobile app. The app also allows users to send commands (e.g., turning on/off pins) back to the device through Firestore, creating two-way real-time interaction.

---

## Project Components

### 1. ESP32 Firmware

- **Purpose:** Collect sensor data and send it via MQTT; receive commands to control onboard GPIOs.
- **Features:**
  - Connects to Wi-Fi (configurable SSID and password).
  - Connects securely to HiveMQ Cloud MQTT broker (`mqtts://`) using TLS with certificate verification.
  - Publishes randomized sensor values (`temperature`, `humidity`, `light`) to MQTT topics.
  - Subscribes to command topic (`home/temperature`) and changes WS2812 RGB LED color based on commands:
    - `"increase"` → Green LED
    - `"decrease"` → Red LED
    - Others → Blue LED
- **Technologies:**
  - ESP-IDF framework with FreeRTOS
  - MQTT client from ESP-IDF
  - WS2812 LED control using RMT peripheral
- **Customization:**
  - Wi-Fi credentials in `wifi_config.h`
  - MQTT broker URI, username, password in `main.c`
  - LED GPIO pin and count configurable via macros

---

### 2. MQTT Broker (HiveMQ Cloud)

- Provides a secure, reliable MQTT messaging platform.
- Broker details configured in ESP32 firmware.
- Facilitates MQTT publish-subscribe communication between the ESP32 and Node-RED.

---

### 3. Node-RED Flow

- Acts as a middleware to bridge MQTT and Firestore.
- Subscribes to MQTT sensor data topics (`esp/in/temperature`, `esp/in/humidity`, `esp/in/light`).
- Writes incoming data to corresponding Firestore documents inside the `esp32` collection.
- Listens to Firestore `commands` collection for user commands from the mobile app.
- When a command is detected, Node-RED publishes the command back to MQTT topic (`home/temperature`), which ESP32 listens to.

---

### 4. Firebase Firestore

- Cloud-hosted NoSQL database storing sensor readings and commands.
- Firestore collections:
  - `esp32`: Contains documents like `temperature`, `humidity`, `light` with current sensor values.
  - `commands`: Stores user commands (`increase`/`decrease`) with timestamps.
- Enables real-time synchronization between the device and the mobile app.
- Security rules can be customized for read/write access.

---

### 5. Flutter Mobile App

- Cross-platform mobile app built with Flutter and Dart.
- Connects to Firestore to stream sensor data live.
- Displays sensor values dynamically in a list view.
- Provides UI buttons to send `increase` or `decrease` commands.
- Commands are written into Firestore `commands` collection with timestamp.
- User commands are then relayed back to the ESP32 via Node-RED and MQTT.

---

## How It Works Together

1. **ESP32 collects sensor data** → publishes via MQTT → HiveMQ broker.
2. **Node-RED subscribes to MQTT topics** → updates Firestore with sensor data.
3. **Flutter app listens to Firestore** → displays sensor data live.
4. User **presses buttons in Flutter app** → writes command to Firestore.
5. **Node-RED watches Firestore commands** → publishes MQTT command message.
6. **ESP32 receives MQTT command** → changes LED color or pin state.

---

## Setup Instructions

### ESP32 Firmware

- Configure Wi-Fi and MQTT credentials.
- Compile and flash the firmware using ESP-IDF.
- Ensure HiveMQ Cloud MQTT broker is set up with TLS cert.

### Node-RED

- Import the provided Node-RED flow (subscribe MQTT → Firestore, Firestore → publish MQTT).
- Configure MQTT connection to HiveMQ Cloud.
- Configure Firebase Admin credentials for Firestore access.

### Firebase Firestore

- Create a Firebase project.
- Enable Firestore in Native mode.
- Set up collections: `esp32` (sensor data), `commands` (control commands).
- Add necessary security rules.

### Flutter App

- Add Firebase config files (`google-services.json` and `GoogleService-Info.plist`).
- Run `flutter pub get` to install dependencies.
- Launch the app on an Android/iOS device or emulator.

---

## Code Highlights

- **ESP32 Firmware**: Wi-Fi and MQTT setup with event handlers, LED control based on MQTT messages.
- **Node-RED**: Uses MQTT input/output nodes and Firestore read/write nodes with logic for bidirectional data flow.
- **Flutter App**: Uses Firestore `StreamBuilder` for real-time UI updates and Firestore writes for command sending.

---

## Potential Improvements

- Add secure user authentication on the mobile app.
- Expand supported sensor types and device controls.
- Implement push notifications for sensor alerts.
- Add offline data caching and synchronization.
- Enhance UI/UX with better state management.

---

## Troubleshooting

- Check Wi-Fi and MQTT connection logs on ESP32 serial console.
- Ensure MQTT broker URI, username, and password are correct.
- Verify Firestore security rules allow read/write from your app and Node-RED.
- Confirm Node-RED credentials for Firebase Admin SDK are valid.
- Use Firebase console to monitor collections and real-time data changes.

---
## License

This project is licensed under the MIT License. See the LICENSE file for details.

---

## Acknowledgments

Special thanks to the open source community and the official docs of ESP-IDF, Node-RED, Firebase, HiveMQ, and Flutter for enabling this project.

---

If you want help setting up any part or need clarifications, feel free to ask!

--- 
## 👨‍💻 Author  Developed by: **[Halime Türkmen]**   Date: May 2025