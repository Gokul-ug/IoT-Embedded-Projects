// 🔹 Updated Blynk Credentials
#define BLYNK_TEMPLATE_NAME "Gas Fire Safety System"
#define BLYNK_AUTH_TOKEN "PHQvSKYdu84e8dqBFUdXzhrN__WmyiSa"
#define BLYNK_TEMPLATE_ID "TMPL3lH-7ayPE"

#include <ESP32Servo.h>
#include <DHT.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include "gas_leakage_model.h"  // ML-based leakage probability logic

const char* ssid = "Gtech";
const char* pass = "12345678";

// 🔹 Sensor & Actuator Pins
const int gasSensorPin = 34;
const int flameSensorPin = 14;
const int buzzerPin = 27;
const int servoPin = 26;
const int fanPin = 25;
const int motorRelayPin = 33;

const int debounceCount = 5;
const int flameThreshold = 3;

#define DHTPIN 13
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

Servo myServo;
bool servoMoved = false;
bool motorActive = false;
bool fanActive = false;

unsigned long buzzerStartTime = 0;  // Tracks buzzer start time
bool buzzerActive = false;          // Tracks buzzer status

// 🔹 Blynk Virtual Pin Controls
BLYNK_WRITE(V4) { controlFan(param.asInt()); }
BLYNK_WRITE(V5) { controlMotor(param.asInt()); }

void setup() {
    Serial.begin(115200);

    WiFi.begin(ssid, pass);
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

    pinMode(gasSensorPin, INPUT);
    pinMode(flameSensorPin, INPUT);
    pinMode(buzzerPin, OUTPUT);
    pinMode(fanPin, OUTPUT);
    pinMode(motorRelayPin, OUTPUT);

    myServo.attach(servoPin);
    myServo.write(0);  // Open position
    servoMoved = false;

    digitalWrite(buzzerPin, LOW);
    digitalWrite(fanPin, HIGH);
    digitalWrite(motorRelayPin, HIGH);

    analogReadResolution(12);
    dht.begin();
}

// 🔹 Buzzer Control (Non-blocking)
void activateBuzzer() {
    if (!buzzerActive) {
        digitalWrite(buzzerPin, HIGH);
        buzzerStartTime = millis();
        buzzerActive = true;
    }
}

void manageBuzzer() {
    if (buzzerActive && millis() - buzzerStartTime >= 5000) {
        digitalWrite(buzzerPin, LOW);
        buzzerActive = false;
    }
}

// 🔹 Fan Control
void controlFan(bool state) {
    if (state && !fanActive) {
        digitalWrite(fanPin, LOW);
        Serial.println("🌀 Fan ON");
        fanActive = true;
        Blynk.virtualWrite(V4, 1);
    } else if (!state && fanActive) {
        digitalWrite(fanPin, HIGH);
        Serial.println("🛑 Fan OFF");
        fanActive = false;
        Blynk.virtualWrite(V4, 0);
    }
}

// 🔹 Water Motor Control
void controlMotor(bool state) {
    if (state && !motorActive) {
        digitalWrite(motorRelayPin, LOW);
        Serial.println("🔥 Water Motor ON!");
        motorActive = true;
        Blynk.virtualWrite(V5, 1);
    } else if (!state && motorActive) {
        digitalWrite(motorRelayPin, HIGH);
        Serial.println("🚰 Water Motor OFF!");
        motorActive = false;
        Blynk.virtualWrite(V5, 0);
    }
}

// 🔹 Sensor Data Handling
void sendSensorData() {
    int gasLevel = analogRead(gasSensorPin);
    int flameDetected = digitalRead(flameSensorPin);
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("❌ DHT sensor error! Using safe defaults.");
        temperature = 32.0;
        humidity = 50.0;
    }

    int leakageProbability = getLeakageProbability(gasLevel);

    Blynk.virtualWrite(V0, temperature);
    Blynk.virtualWrite(V1, humidity);
    Blynk.virtualWrite(V2, gasLevel);
    Blynk.virtualWrite(V3, leakageProbability);
    Blynk.virtualWrite(V6, flameDetected == 0 ? 1 : 0);

    Serial.printf("🌡️ Temp: %.1f°C | 💧 Humidity: %.1f%%\n", temperature, humidity);
    Serial.printf("🟠 Gas Level: %d | 📊 Leakage Probability: %d%%\n", gasLevel, leakageProbability);
    Serial.printf("🔥 Flame Sensor: %s\n", flameDetected ? "No Fire" : "🔥 Fire Detected!");
}

// 🔹 Gas & Fire Detection Logic
void checkSafetyConditions() {
    int gasLevel = analogRead(gasSensorPin);
    int leakageProbability = getLeakageProbability(gasLevel);
    int stableReadings = 0;

    if (leakageProbability > 80) {
        Serial.println("🚨 High gas detected! Closing servo and activating fan.");
        activateBuzzer();
        controlFan(true);

        if (!servoMoved) {
            myServo.write(90);
            servoMoved = true;
        }
        Blynk.logEvent("gas_leak_alert", "🚨 High Gas Leak Detected!");
    } else {
        controlFan(false);
    }

    for (int i = 0; i < debounceCount; i++) {
        if (digitalRead(flameSensorPin) == 0) {
            stableReadings++;
        }
        delay(50);
    }

    if (stableReadings >= flameThreshold) {
        Serial.println("🔥 Fire detected! Turning on water motor.");
        activateBuzzer();
        controlMotor(true);
        Blynk.logEvent("fire_alert", "🔥 Fire Detected! Water Motor Activated.");
    } else {
        controlMotor(false);
    }

    manageBuzzer();
}

BlynkTimer timer;

void loop() {
    Blynk.run();
    timer.run();

    sendSensorData();
    checkSafetyConditions();
    delay(2000);  // Prevent CPU overload
}
