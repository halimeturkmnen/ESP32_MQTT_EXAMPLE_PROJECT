# Smart Home Flutter App

## Project Overview

The **Smart Home Flutter App** is a mobile application developed with Flutter that seamlessly integrates with an ESP32 IoT device via Firebase Firestore. The app displays sensor data sent by the ESP32 through MQTT, relayed to Firestore, and provides real-time control over the device’s GPIO pins. Users can toggle pins ON/OFF by sending commands (`increase` or `decrease`) from the app, which are then written to Firestore and consumed by the ESP32.

This project demonstrates a full-stack IoT solution combining Flutter, Firebase, and MQTT technologies to create an interactive and responsive smart home interface.

---

## Features

- **Real-Time Data Streaming:** Sensor data (temperature, humidity, light, etc.) sent by the ESP32 is updated live on the app using Firestore's real-time listeners.
- **Two-Way Communication:** Commands issued in the app update Firestore’s `commands` collection, which the ESP32 listens to for controlling its pins.
- **Firebase Firestore Integration:** Utilizes Firestore as a NoSQL database to store sensor readings and commands efficiently.
- **Flutter UI:** Intuitive, clean, and responsive user interface designed with Flutter for smooth user experience on Android (and iOS).
- **Robust and Scalable:** Leverages Firebase's backend infrastructure for scalable data handling and app performance.

---

## Technologies Used

| Technology       | Purpose                                    |
|------------------|--------------------------------------------|
| Flutter          | Cross-platform mobile app development      |
| Dart             | Programming language for Flutter           |
| Firebase Core    | Core Firebase services integration         |
| Cloud Firestore  | Real-time NoSQL cloud database              |
| ESP32            | IoT device publishing sensor data via MQTT|
| MQTT Broker      | Messaging protocol for IoT communication   |

---
## Setup & Installation

### Prerequisites

- Flutter SDK installed and configured (with Flutter in your system PATH)
- Android Studio, VSCode, or other Flutter-compatible IDE
- Firebase project with Firestore enabled
- Firebase SDK integrated into your Flutter project

### Steps to Run

1. Clone or download the repository:

    ```bash
    git clone <repository-url>
    cd flutter_app/smart_home
    ```

2. Install Flutter dependencies:

    ```bash
    flutter pub get
    ```

3. Set up Firebase Firestore:

   - Create `esp32` and `commands` collections in your Firestore database.
   - Add necessary security rules to allow your app to read/write these collections securely.

4. Add Firebase config files to your project:

   - `google-services.json` to `android/app/`
   - `GoogleService-Info.plist` to `ios/Runner/`

5. Run the app on an emulator or physical device:

    ```bash
    flutter run
    ```

---

## How to Use

- The app shows a list of sensor values from the `esp32` collection in Firestore.
- Each list item displays the sensor type and its current value.
- Tap the green plus (`+`) button to send an `increase` command, or tap the red minus (`-`) button to send a `decrease` command.
- These commands are written to the `commands` collection in Firestore.
- Your ESP32 device listens to this collection and acts accordingly by toggling the GPIO pins.
- The sensor values update in real-time as the device publishes new data.

---

## Code Explanation (`main.dart`)

- Initializes Firebase asynchronously on app startup.
- Uses a `StreamBuilder` to listen to Firestore’s `esp32` collection and dynamically update the UI.
- Displays sensor data as a scrollable list with increase/decrease command buttons.
- Command button presses trigger `_sendCommand()`, writing commands with timestamps to Firestore.
- UI automatically reflects data and command updates without manual refresh.

---

## Recommendations for Further Development

- Expand the app to support more sensor types or device controls.
- Implement user authentication to secure access.
- Use advanced state management (Provider, BLoC, Riverpod) for better scalability.
- Improve Firestore security rules according to your application’s needs.
- Add notifications or alerts based on sensor thresholds.
- Optimize UI for tablets and different screen sizes.

---

## Contact

For questions, suggestions, or contributions, please reach out to the project maintainer.

---

## License

This project is licensed under the MIT License. See the LICENSE file for details.

---

## Acknowledgments

Thanks for exploring this Smart Home project! Combining Flutter, Firebase, and IoT technologies opens powerful possibilities for home automation and real-time monitoring.

---
