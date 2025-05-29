# 🛡️ Intruder Alert System with PIR and Laser-LDR Sensors

## 🔍 Overview
This enhanced Intruder Alert System detects unauthorized presence using two types of sensors — a **PIR motion sensor** and a **Laser-LDR beam-break sensor**. It offers reliable detection, especially in sensitive or high-security areas, and can also send real-time alerts via the Blynk IoT app.

## 🧰 Components Used
- NodeMCU / ESP32 / Arduino UNO
- PIR Motion Sensor
- LDR (Light Dependent Resistor)
- Laser Module
- Buzzer
- LED (optional)
- Relay (for external siren or lights)
- Resistors, Jumper Wires
- Blynk IoT App 

## ⚙️ How It Works
1. **Laser-LDR Beam Setup**:
   - A laser points directly at the LDR.
   - When the beam is broken (e.g., someone crosses), resistance changes are detected.

2. **PIR Motion Detection**:
   - Detects human body movement in the field of view.

3. **Alert Mechanism**:
   - If **either sensor is triggered**, the system:
     - Sounds a buzzer
     - (Optional) Sends a notification via Blynk
     - Activates an LED or alarm light


## 📲 Blynk App Setup (Optional)
- Add Notification and LED widgets
- Use `Blynk.notify("Intruder Detected!")`
- Use virtual pins for remote status or disabling alarms

---

## ✅ Features
- Dual-layer detection: **Laser-LDR** + **PIR sensor**
- Audible and visual alerts via buzzer/LED
- Optional mobile notifications using IoT
- Great for home entrances, lockers, or corridors
---
## 🛠️ Future Enhancements
- Add ESP32-CAM for face/image capture
- Use GSM module for SMS alerts
- Integrate with home automation systems
