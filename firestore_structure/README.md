# Firestore Structure Documentation

This document explains the structure and logic behind the Firestore database used in this ESP32 MQTT project. It is designed to help beginners understand how data is organized, stored, and synchronized between the ESP32 device, the MQTT broker, and the Flutter application.

---

## Overview

The Firestore database organizes data into collections and documents that represent sensor readings from the ESP32 device as well as user commands sent from the Flutter app.

The JSON file `firestore_structure.json` illustrates the hierarchical layout of the Firestore documents, including field names, expected data types, and how each section is related.

---

## Firestore Structure Breakdown

```json
{
  "esp32": {
    "humidity": {
      "timestamp": "timestamp (ISO 8601)",
      "value": "number"
    },
    "temperature": {
      "timestamp": "timestamp (ISO 8601)",
      "value": "number"
    },
    "light": {
      "timestamp": "timestamp (ISO 8601)",
      "value": "number"
    }
  },
  "commands": {
    "humidity": {
      "command": "string (e.g., 'decrease')",
      "timestamp": "human-readable string (e.g., May 2, 2025 at 2:13:16 PM UTC+3)"
    },
    "temperature": {
      "command": "string",
      "timestamp": "string"
    },
    "light": {
      "command": "string",
      "timestamp": "string"
    }
  }
}

# 1. esp32 Collection

This collection stores sensor data published by the ESP32 device to MQTT topics.

It contains three documents representing different sensor types:

- **humidity**
- **temperature**
- **light**

Each sensor document contains the following fields:

| Field     | Description                             | Format / Type                                 |
|-----------|-----------------------------------------|-----------------------------------------------|
| timestamp | The exact time when the sensor data was recorded | ISO 8601 timestamp string (e.g., "2025-05-27T14:30:00Z") |
| value     | The numeric value read from the sensor  | Number (e.g., 45.3 for humidity)              |

---

# 2. commands Collection

This collection stores commands sent by the user from the Flutter app through Firestore.

These commands are then forwarded to the ESP32 device via MQTT topics that the device subscribes to.

It includes documents for each sensor type (humidity, temperature, light), representing control commands like "increase" or "decrease".

Each command document contains the following fields:

| Field     | Description                          | Format / Type                                     |
|-----------|--------------------------------------|---------------------------------------------------|
| command   | The command string sent by the user | String (e.g., "increase" or "decrease")          |
| timestamp | The time when the command was issued | Human-readable string (e.g., "May 2, 2025 at 2:13:16 PM UTC+3") |

---

# How Data Flows in This System

## ESP32 Sensor Data Publishing

- The ESP32 device reads sensor values for humidity, temperature, and light.
- It publishes these values to respective MQTT topics.
- Node-RED or another MQTT-to-Firestore bridge listens to these topics and writes the data to Firestore under the `esp32` collection, updating the corresponding documents with the latest value and timestamp.

## User Commands from Flutter App

- The Flutter app presents on/off buttons or sliders corresponding to sensor controls.
- When the user interacts with these UI elements, commands such as "increase" or "decrease" are written to Firestore under the `commands` collection.
- Another MQTT bridge or backend service listens for changes in Firestore's `commands` documents.
- It forwards these commands to the MQTT broker, publishing on topics the ESP32 subscribes to.
- The ESP32 receives these commands and adjusts its behavior accordingly (e.g., turning on/off a device or adjusting thresholds).

---

# Important Notes

## Timestamps:
- Timestamps in sensor readings use the standard ISO 8601 format to ensure universal and unambiguous time representation.
- User command timestamps are stored in a human-readable format for easier debugging and logging.

## Data Consistency:
- Firestore's real-time syncing allows the Flutter app to instantly reflect sensor updates and command status, providing seamless two-way communication.

## Extensibility:
- This structure can easily be expanded by adding new sensor types or commands, simply by adding new documents under the `esp32` or `commands` collections with the same field schema.

---

# Summary

This Firestore structure creates a clean separation between sensor data (from ESP32) and control commands (from the user), facilitating a reactive, real-time IoT system. By following this schema, developers can build applications that tightly integrate hardware, cloud, and user interfaces with minimal complexity.
