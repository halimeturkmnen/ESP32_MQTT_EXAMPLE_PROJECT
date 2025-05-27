# 📡 Node-RED + ESP32 + MQTT + Firestore Project

This project connects an **ESP32 device** with **Google Firestore** using **MQTT** and **Node-RED**. The ESP32 sends temperature, humidity, and light sensor data via MQTT. Node-RED receives this data and writes it to Firestore. It also listens for commands from Firestore and sends them back to the ESP32 over MQTT.

## 🧠 Core Components

| Component | Role |
|----------|------|
| ESP32 | Collects sensor data and publishes it via MQTT |
| MQTT Broker (HiveMQ Cloud) | Middleware for message exchange |
| Node-RED | Handles logic: receives MQTT, writes to Firestore, reads Firestore and sends MQTT commands |
| Firebase Firestore | Cloud database for sensor values and control commands |

---

## 🚀 Getting Started

### 1. Requirements

- [Node-RED](https://nodered.org/docs/getting-started/)
- [HiveMQ Cloud MQTT Broker](https://www.hivemq.com/mqtt-cloud-broker/)
- [Firebase](https://firebase.google.com/)
- Node-RED nodes: `node-red-contrib-cloud-firestore`, `node-red-contrib-firebase-admin`
- ESP32 device (e.g., ESP32-C3-DevKitM-1)

---

## 📦 Project Structure

```
node_red_flows/
├── flows.json  ← Node-RED flow file
└── README.md   ← This file
```

---

## 🌐 MQTT Topics

| MQTT Topic (ESP32 → Node-RED) | Description |
|------------------------------|-------------|
| `esp/in/temperature`         | Temperature data |
| `esp/in/humidity`            | Humidity data |
| `esp/in/light`               | Light data |

| MQTT Topic (Node-RED → ESP32) | Description |
|------------------------------|-------------|
| `home/temperature`           | Temperature control command |
| `home/humidity`              | Humidity control command |
| `home/light`                 | Light control command |

---

## 🗃️ Firestore Structure

### Collections and Documents:

- `esp32`
  - `temperature`: Latest temperature reading
  - `humidity`: Latest humidity reading
  - `light`: Latest light reading

- `commands`
  - `temperature`: Command for temperature
  - `humidity`: Command for humidity
  - `light`: Command for light

---

## 🧩 Node-RED Flow Breakdown

### 🔄 Receiving MQTT and Writing to Firestore

#### Example: Temperature

1. **MQTT In Node**
   - Topic: `esp/in/temperature`

2. **Function Node**
```javascript
msg.payload = {
    value: parseFloat(msg.payload),
    timestamp: new Date().toISOString()
};
msg.documentPath = "esp32/temperature";
return msg;
```

3. **Firestore Out Node**
   - Collection: `esp32`
   - Document: `temperature`
   - Operation: `set`

#### Humidity and Light Flows are similar

Just change topics and paths accordingly.

---

### 🔁 Reading Commands from Firestore and Sending to MQTT

#### Example: Temperature Command

1. **Firestore In Node**
   - Collection: `commands`
   - Document: `temperature`
   - Realtime: Enabled

2. **Function Node**
```javascript
var data = msg.payload;
msg.topic = "home/temperature";
msg.payload = data.command;
return msg;
```

3. **MQTT Out Node**
   - Topic: `home/temperature`

#### Humidity and Light Commands work similarly.

---

## 🔒 MQTT Broker Settings (HiveMQ Cloud)

```json
{
  "broker": "7ab1e9bf6329443886532525b88adc36.s1.eu.hivemq.cloud",
  "port": "8883",
  "usetls": true,
  "clientid": "esp32hivemq"
}
```

Username and password must be configured in the MQTT broker node in Node-RED.

---

## 🔥 Firebase Admin Setup

1. Go to Firebase > Project Settings > Service accounts
2. Generate a new private key and download the JSON file
3. Load this JSON into the `firebase-admin` node in Node-RED

Ensure the node is labeled `esp32ProjectFirestore`.

---

## 🛠️ Importing the Flow

1. Open Node-RED
2. Click menu > `Import > Clipboard`
3. Paste the contents of `flows.json`
4. Click `Import`

---

## 📲 ESP32 Logic

Your ESP32 sketch should:

- Connect to Wi-Fi and HiveMQ MQTT broker (`mqtts://`)
- Publish sensor data to `esp/in/...` topics
- Subscribe to `home/...` topics and act on commands (e.g., toggle LEDs)

---

## 📌 Summary

This system bridges ESP32 sensors with Firestore using MQTT and Node-RED. Data is formatted with timestamps and stored in Firestore. Commands can be issued in real-time from Firestore and applied on the ESP32.


