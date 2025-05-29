#define BLYNK_TEMPLATE_ID "TMPL3bcTT6vqs"
#define BLYNK_TEMPLATE_NAME "Intruder Alert System"
#define BLYNK_AUTH_TOKEN "1t8rovc_eGOMOQaKBq8BuonVvHNfwORl"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Wi-Fi and Blynk credentials
char ssid[] = "Gtech";          // Replace with your Wi-Fi SSID
char pass[] = "12345678";      // Replace with your Wi-Fi password

// Pin definitions
#define REED_PIN 13      // Reed Sensor
#define LDR_PIN 34       // LDR Sensor
#define PIR_PIN 14       // PIR Sensor
#define FORCE_PIN 35     // Force Sensor
#define BUZZER_PIN 27    // Buzzer
#define LED_PIN 26       // Red LED

// Sensor control variables
bool reedEnabled = false;
bool pirEnabled = false;
bool ldrEnabled = false;
bool forceEnabled = false;

// Blynk Button Handlers for Sensor Control
BLYNK_WRITE(V0) { reedEnabled = param.asInt(); }  // ReedDoorControl
BLYNK_WRITE(V1) { pirEnabled = param.asInt(); }   // PIR_Motion_Detection
BLYNK_WRITE(V2) { ldrEnabled = param.asInt(); }   // LDR_Laser_Control
BLYNK_WRITE(V3) { forceEnabled = param.asInt(); } // Force_Control

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(REED_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(FORCE_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  Serial.println("Intruder Alert System with Blynk Control Initialized");
}

// Function to Trigger Alarm with Specific Sensor Name
void triggerAlarm(String sensorName) {
  digitalWrite(BUZZER_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);
  Blynk.virtualWrite(V4, HIGH);  // LED in Blynk app

  // Send specific alert notification
  String alertMessage = "Intruder Alert! " + sensorName + " Triggered!";
  Blynk.logEvent("alert", alertMessage);
  delay(500);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  Blynk.virtualWrite(V4, LOW);  // LED in Blynk app
  delay(500);
}

void loop() {
  int reedState = digitalRead(REED_PIN);
  int pirState = digitalRead(PIR_PIN);
  int ldrValue = analogRead(LDR_PIN);
  int forceValue = analogRead(FORCE_PIN);

  Serial.print("Reed: "); Serial.print(reedState);
  Serial.print(" | PIR: "); Serial.print(pirState);
  Serial.print(" | LDR: "); Serial.print(ldrValue);
  Serial.print(" | Force: "); Serial.println(forceValue);

  // Check enabled sensors and trigger specific alerts
  if (reedEnabled == HIGH && reedState == HIGH) {
    Serial.println("Door Opened - Intruder Alert!");
    triggerAlarm("Reed Sensor");
  }

  if (pirEnabled && pirState == HIGH) {
    Serial.println("Motion Detected - Intruder Alert!");
    triggerAlarm("PIR Sensor");
  }

  if (ldrEnabled && ldrValue > 500) {
    Serial.println("Laser Interrupted - Intruder Alert!");
    triggerAlarm("LDR Sensor");
  }

  if (forceEnabled && forceValue > 0) {
    Serial.println("Footstep Detected - Intruder Alert!");
    triggerAlarm("Force Sensor");
  }

  Blynk.run();  // Keep Blynk connection alive
  delay(1000);
}
