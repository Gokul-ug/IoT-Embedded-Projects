/* Home Automation with LDR, PIR & Blynk Control for 3 Relays with Hybrid Logic */

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL3Zv989TtQ"
#define BLYNK_TEMPLATE_NAME "Smart Lighting system for classroom"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Define relay, LDR, and PIR pins
#define relay1 D0
#define relay2 D1
#define relay3 D2
#define LDR_PIN A0     // Analog pin for LDR sensor
#define PIR_PIN D3     // Digital pin for PIR sensor

#define BLYNK_AUTH_TOKEN "mlqSe8rtme3Q7sxDDSv84OzRkf7v4MHF"

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Akashtech";     // Enter your Wi-Fi SSID
char pass[] = "12345678";      // Enter your Wi-Fi password

int ldrThreshold = 500;        // Light level threshold (adjust as needed)
bool relay1State = HIGH;       // Default relay states (HIGH = OFF for active-low relay)
bool relay2State = HIGH;
bool relay3State = HIGH;
bool motionDetected = false;   // PIR motion detection flag

unsigned long lastManualTime = 0;         // Timestamp for last manual control
unsigned long motionStartTime = 0;        // Timestamp for last motion
const int manualTimeout = 5000;           // Manual override timeout (5 seconds)
const unsigned long pirTimeout = 120000;  // Motion timeout (2 minutes)

// Blynk Button for Relay 1 (V0)
BLYNK_WRITE(V0) {
  relay1State = param.asInt() ? LOW : HIGH;
  digitalWrite(relay1, relay1State);
  lastManualTime = millis();
}

// Blynk Button for Relay 2 (V1)
BLYNK_WRITE(V1) {
  relay2State = param.asInt() ? LOW : HIGH;
  digitalWrite(relay2, relay2State);
  lastManualTime = millis();
}

// Blynk Button for Relay 3 (V2)
BLYNK_WRITE(V2) {
  relay3State = param.asInt() ? LOW : HIGH;
  digitalWrite(relay3, relay3State);
  lastManualTime = millis();
}

void setup() {
  Serial.begin(115200);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(PIR_PIN, INPUT);

  // Set initial relay states
  digitalWrite(relay1, relay1State);
  digitalWrite(relay2, relay2State);
  digitalWrite(relay3, relay3State);

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
}

void loop() {
  Blynk.run();

  unsigned long currentMillis = millis();

  // If no manual action in the last 5 seconds, allow sensor control
  if (currentMillis - lastManualTime > manualTimeout) {

    int ldrValue = analogRead(LDR_PIN);
    int pirValue = digitalRead(PIR_PIN);

    Serial.print("LDR: ");
    Serial.print(ldrValue);
    Serial.print(" | PIR: ");
    Serial.println(pirValue);

    // If it's dark AND motion is detected
    if (ldrValue > (ldrThreshold - 50) && pirValue == HIGH) {
      motionDetected = true;
      motionStartTime = currentMillis;
      Serial.println("Dark + Motion Detected - Lights ON");

      relay1State = LOW;
      relay2State = LOW;
      relay3State = LOW;
    }

    // Turn OFF lights after timeout OR if it gets bright
    if ((motionDetected && (currentMillis - motionStartTime > pirTimeout)) || ldrValue < (ldrThreshold + 50)) {
      motionDetected = false;
      Serial.println("No Motion or Bright - Lights OFF");

      relay1State = HIGH;
      relay2State = HIGH;
      relay3State = HIGH;
    }

    // Apply relay states
    digitalWrite(relay1, relay1State);
    digitalWrite(relay2, relay2State);
    digitalWrite(relay3, relay3State);
  }

  delay(500);  // Adjust for smooth loop timing
}
