# 🔥 Gas Leakage Detection System

## 🔍 Overview
This project is an IoT-based **gas leakage detection system** that senses dangerous gases like LPG or Methane using an **MQ sensor** and takes immediate action by activating safety systems like a **buzzer, fan, servo-controlled gas valve**, and sends alerts to the user via Blynk or similar platforms.

## 🧰 Components Used
- ESP8266 (NodeMCU)
- MQ-2 / MQ-5 / MQ-6 Gas Sensor
- Buzzer
- Servo Motor (for gas valve)
- Relay + Exhaust Fan
- 16x2 LCD (optional)
- LED indicators
- Blynk App / IoT platform

---

## ⚙️ Working Principle

1. **Detection**:
   - The MQ sensor continuously monitors the environment for gas leakage.
   - Analog values are read and compared against a threshold.

2. **Response**:
   - If a leak is detected:
     - Buzzer is turned ON.
     - Fan is activated to vent out gas.
     - Servo turns OFF gas supply.
     - Red LED is lit.
     - An alert is sent via Blynk.

3. **User Interface**:
   - Gas level displayed on LCD or Blynk
   - Manual reset and control through app (optional)

---

## ✅ Features
- Real-time gas monitoring
- Buzzer + Fan + Servo-based safety action
- Mobile notifications via Blynk
- Visual alert via LED and LCD
- Compact and responsive system

---

## 🚀 Future Improvements
- Add flame sensor for fire detection
- Cloud logging of gas readings
- SMS/email alerts via Twilio or IFTTT
