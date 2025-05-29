# 🧪 IoT-Enabled Gas Detection System with TinyML for Smart Safety Solutions

## 🔍 Overview
This project leverages TinyML to build an intelligent gas detection system that can identify potential gas leakages and take real-time safety actions. It uses an MQ-series gas sensor to capture gas concentration data, which is processed locally using a trained TinyML model on the microcontroller to determine whether the environment is safe or hazardous. In case of gas detection, a buzzer is triggered, a fan is activated, and a servo closes the gas supply. All activity is also logged and visualized on the Blynk app.

## 🧠 Powered by TinyML
- A lightweight ML model was trained to classify **gas levels** as `Safe`, `Warning`, or `Danger`.
- The model is converted to a `.tflite` format and deployed on **ESP32/Arduino** using **TensorFlow Lite for Microcontrollers (TFLM)**.
- This approach enables **low-latency**, **low-power**, and **offline** inference at the edge.

## 🧰 Components Used
- ESP32
- MQ-2 Gas Sensor
- Servo Motor
- Buzzer
- Fan (via relay)
- LCD Display (I2C)
- Red/Green LEDs
- Blynk App (IoT visualization)
- TensorFlow Lite for Microcontrollers (TinyML)

## ⚙️ How It Works
1. **Sensor Data**: MQ sensor reads gas concentration in ppm.
2. **Inference**: The TinyML model predicts the safety level:
   - Safe (Green LED)
   - Warning (Yellow LED)
   - Danger (Red LED, Fan, Servo, Buzzer, Alert)
3. **IoT Monitoring**: System sends live readings and alerts to the Blynk app for remote monitoring.

## 📈 Model Details
- **Input Features**: Raw ADC value or calibrated gas level (ppm)
- **Classes**: Safe, Warning, Danger
- **Model Type**: Tiny neural network
- **Tools Used**: TensorFlow, Arduino IDE, Edge Impulse (optional)

## 🖼️ Circuit Diagram
![Circuit Diagram](./circuit_diagram.png)

## 💻 Code & Model
- Main Code: [code.ino](./code.ino)
- TFLite Model: [model.tflite](./model.tflite)

## 📲 Blynk App Setup
- Add Gauge, LED, and Notification widgets
- Set up Virtual Pins for interaction and alerts
- Update `BLYNK_AUTH_TOKEN` in the code

---

## ✅ Features
- Real-time intelligent classification of gas levels
- On-device ML using TinyML
- Safety automation using actuators
- IoT dashboard integration (Blynk)
