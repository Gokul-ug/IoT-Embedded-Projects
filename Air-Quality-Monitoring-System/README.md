# 🌫️ Air Quality Monitoring System

## 🔍 Overview
This project is an IoT-enabled system that monitors air pollution levels using sensors like **MQ135**, **MQ2**, or **MQ7**. It provides real-time data visualization through an LCD display and optionally sends data to the **Blynk IoT app** for remote monitoring and alerts.

## 🧰 Components Used
- ESP8266 / NodeMCU
- MQ135 / MQ2 / MQ7 / PM2.5 Gas Sensor
- DHT11/DHT22 for humidity & temperature
- 16x2 LCD (I2C)
- Buzzer (for alert)
- LED (Air quality status)
- Blynk App for real-time remote monitoring
- Power supply or battery bank

---

## ⚙️ Working Principle

1. **Sensor Readings**:
   - MQ sensors detect toxic gases (e.g., CO₂, NH₃, smoke).
   - DHT11/DHT22 measures humidity and temperature.

2. **Display & Feedback**:
   - Air quality and temperature displayed on the LCD.
   - Buzzer + LED used to indicate poor air quality.

3. **Remote Monitoring**:
   - Values sent to Blynk dashboard (Graph, Gauge, LED, Notification).
   - Alerts sent if gas level exceeds threshold.

---

## ✅ Features
- Real-time gas level monitoring
- Alerts for unsafe air conditions
- Temperature & humidity data
- Remote Blynk-based dashboard
- LCD display for local visibility

---

## 🛠️ Future Enhancements
- GPS + map of pollution sources
- Data logging to Google Sheets
- Air quality index (AQI) calculation
