# 🩺 Dynamic Health Surveillance System

## 🔍 Overview
This project is an IoT-enabled health surveillance system that continuously monitors critical health parameters such as **heart rate**, **body temperature**, and **room temperature**. It's ideal for patient monitoring, elderly care, and remote health diagnostics. The data is displayed on an LCD and sent to a **Blynk dashboard** in real-time.

## 🧰 Components Used
- ESP8266 
- Heartbeat Sensor (KY-039 or Pulse Sensor)
- Temperature Sensor (LM35 / DHT11 / DS18B20)
- LCD Display (I2C)
- Buzzer and LED (for alerts)
- WiFi Module (built-in or ESP-01)
- Blynk App (for real-time data monitoring)

## ⚙️ How It Works
1. **Heart Rate Monitoring**:
   - Pulse sensor detects beats per minute (BPM).
   - If BPM < threshold → alert via buzzer/LED.

2. **Temperature Monitoring**:
   - Body and room temperature are read from the sensor.
   - If body temperature > 37.5°C → triggers alert.

3. **IoT Monitoring**:
   - Data is sent to Blynk for visualization.
   - Emergency alerts can be sent via app notification.

## 📲 Blynk App Setup
- Add widgets:
  - Gauge (BPM)
  - Gauge (Temperature)
  - LED Indicator
  - Notification Widget
- Use Virtual Pins for data (e.g., V0 for BPM, V1 for Temp)

---

## ✅ Features
- Real-time monitoring of heart rate and temperature
- Local alerts (buzzer + LED)
- Remote monitoring via Blynk IoT app
- LCD display for live updates

---
## 🛠️ Future Enhancements
- Add SpO2 sensor (e.g., MAX30100)
- Integrate with cloud dashboards (e.g., Firebase, ThingsBoard)
- SMS/Email alerts via IFTTT or Twilio
