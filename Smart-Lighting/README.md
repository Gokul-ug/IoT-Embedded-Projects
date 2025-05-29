# 💡 Smart Lighting System

## 🔍 Overview
This Smart Lighting System project automates the control of lights based on the presence of a person (motion detection) and the level of ambient light. It helps in reducing power consumption and is ideal for smart homes, offices, or public spaces.

## 🧰 Components Used
- NodeMCU / ESP32 / Arduino UNO
- PIR Motion Sensor
- LDR (Light Dependent Resistor)
- Relay Module
- LED Bulb / Light
- Jumper Wires
- Blynk App (optional for monitoring/control)

## ⚙️ Working Principle
1. The LDR checks the ambient light.
2. The PIR sensor detects human presence.
3. If motion is detected **and** light level is low:
   - The relay is triggered to turn ON the light.
4. Lights turn OFF automatically when there's no movement or it's bright outside

## 📲 Optional IoT Control
If using Blynk:
- Add Switch and LED widgets
- Monitor lighting status remotely
- Override motion/light detection manually

---

## ✅ Features
- Automatic control of lights
- Power-saving smart operation
- Optional IoT integration via Blynk
