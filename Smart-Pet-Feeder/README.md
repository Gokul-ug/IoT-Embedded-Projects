# 🐾 Smart Pet Feeder (IoT + Timed + Pet Presence Detection)

## 🔍 Overview
A fully automated and IoT-enabled Smart Pet Feeder that dispenses food at scheduled times and detects whether the pet actually eats. It includes manual feed control via the Blynk app and uses an ultrasonic sensor to confirm the pet's presence. Alerts are sent if the pet does not respond.

## 🧰 Components Used
- ESP8266 (NodeMCU)
- Servo Motor (SG90)
- Ultrasonic Sensor (HC-SR04)
- Laser Trigger (to external Uno buzzer system)
- Blynk App (Time Input, Button, Notification)
- Internet Connection

## ⚙️ How It Works

### 🕓 Feeding Logic:
- Three feed times can be configured from the Blynk app using TimeInput widgets (V0, V1, V2).
- Servo rotates to release food at those exact times.
- Manual feed is also possible with a button on V3.

### 🔊 Sound & Motion Feedback:
- A sound signal is sent to another board (e.g., Uno) for buzz alert before/after feeding.
- An ultrasonic sensor checks if the pet approaches the bowl (within 20 cm).
- If the pet does not come near within 10 seconds, an alert is logged to Blynk.

## 📲 Blynk Setup
- **V0, V1, V2** – Time Input for scheduling feeds  
- **V3** – Manual Feed Button  
- **Log Event**: Pet didn’t eat alert  
- Optional: LED or Notification widget for live alerts

---

## ✅ Features
- 3 configurable feeding schedules (via Blynk)
- Manual feed button
- Servo motor for dispensing
- Sound feedback trigger for external speaker
- Pet eating detection using ultrasonic sensor
- Blynk alert if pet doesn’t eat

---

## 🛠️ Future Improvements
- Add food weight sensor for portion tracking
- ESP32-CAM integration to watch feeding
- Log feeding data to Firebase or Google Sheets
  
