#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 or 0x3F based on your module
LiquidCrystal_I2C lcd(0x27, 16, 2); // 16x2 LCD

#include <WiFi.h>
#include <HTTPClient.h>
#include <base64.h>
#include <ESP32Servo.h>

// Sensor & Actuator Pins
#define GAS_SENSOR_PIN 34
#define BUZZER_PIN 4
#define BUTTON_PIN 5
#define RELAY_PIN 18
#define SERVO_PIN 15

// WiFi Credentials
const char* ssid = "Pavan";
const char* password = "hihellohi";

// Twilio Credentials
const char* twilio_account_sid = "ACcd5f31663e407a5f7d4335db3b5708cf";
const char* twilio_auth_token = "54f5092ceed1e098c29c7530bf66eeb7";
const char* twilio_from_number = "+12316266336";
const char* emergency_contact = "+919487860465";
const char* emergency_sms_contact = "+919487860465";

// Global Variables
bool alertTriggered = false;
bool valveClosed = false;
unsigned long lastCallTime = 0;
Servo gasCutoffServo;

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected!");

    pinMode(GAS_SENSOR_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, HIGH);

    gasCutoffServo.attach(SERVO_PIN, 500, 2400);
    gasCutoffServo.write(0); // Initially open

    lcd.init();
    lcd.backlight(); // LCD Backlight ON
    lcd.setCursor(0, 0);
    lcd.print("Gas Detection");
    lcd.setCursor(0, 1);
    lcd.print("System Ready");
    delay(2000);
    lcd.clear();
}

void loop() {
    int gasLevel = analogRead(GAS_SENSOR_PIN);
    Serial.print("Gas Level: "); Serial.println(gasLevel);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Gas Level:");
    lcd.setCursor(0, 1);
    lcd.print(gasLevel);

    // Prevent valve reopening until reset
    if (valveClosed) {
        lcd.setCursor(0, 1);
        lcd.print("Valve Locked!");
        delay(2000);
        return;
    }

    if (gasLevel < 300) {
        alertTriggered = false;
        digitalWrite(RELAY_PIN, HIGH); // Fan OFF
    }

    if (gasLevel >= 300 && !alertTriggered) {
        alertTriggered = true;
        digitalWrite(RELAY_PIN, LOW); // Fan ON
        closeGasValve(); // Lock the valve until reset
        Serial.println("⛔ Gas Cut Off");

        lcd.setCursor(0, 1);
        lcd.print("LEAK DETECTED!");

        alertUser();

        bool canceled = waitForCancel(5000);

        if (!canceled) {
            if ((millis() - lastCallTime) > 120000 || lastCallTime == 0) {
                if (sendEmergencyAlert()) {
                    lastCallTime = millis();
                }
            }
        }

        delay(20000);
        Serial.println("🚫 Valve Locked! Reset Required");
    }

    delay(2000);
    lcd.backlight(); // Keep Backlight ON
}

// Function to close the valve until reset
void closeGasValve() {
    gasCutoffServo.write(90); // Close valve
    valveClosed = true; // Lock the valve
}

// Function to alert the user
void alertUser() {
    Serial.println("⚠️ Gas Leakage Detected!");
    digitalWrite(BUZZER_PIN, HIGH);
    delay(5000);
    digitalWrite(BUZZER_PIN, LOW);
}

// Function to wait for user cancellation
bool waitForCancel(unsigned long timeout) {
    unsigned long startTime = millis();
    while (millis() - startTime < timeout) {
        if (digitalRead(BUTTON_PIN) == LOW) {
            delay(2000);
            if (digitalRead(BUTTON_PIN) == LOW) {
                Serial.println("🚨 Alert Canceled by User!");
                alertTriggered = false;
                digitalWrite(RELAY_PIN, HIGH);
                return true;
            }
        }
    }
    return false;
}

// Emergency alert via Twilio
bool sendEmergencyAlert() {
    String message = "Emergency! Gas leakage detected!";

    if (sendTwilioCall(message, emergency_contact)) {
        Serial.println("📞 Emergency Call Sent!");
    } else {
        Serial.println("❌ Emergency Call Failed!");
    }

    if (sendTwilioSMS(message, emergency_sms_contact)) {
        Serial.println("📩 Emergency SMS Sent!");
    } else {
        Serial.println("❌ Emergency SMS Failed!");
    }

    return true;
}

// Send call using Twilio
bool sendTwilioCall(String message, String recipient) {
    String encodedAuth = "Basic " + base64::encode(String(twilio_account_sid) + ":" + String(twilio_auth_token));
    String twiml = "<Response><Say voice='alice'>" + message + "</Say></Response>";
    String twilio_url = "http://twimlets.com/echo?Twiml=" + URLEncode(twiml);

    String postData = "From=" + String(twilio_from_number) + "&To=" + recipient + "&Url=" + URLEncode(twilio_url);

    HTTPClient http;
    http.begin("https://api.twilio.com/2010-04-01/Accounts/" + String(twilio_account_sid) + "/Calls.json");
    http.addHeader("Authorization", encodedAuth);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int httpResponseCode = http.POST(postData);
    Serial.print("Twilio Call Response Code: ");
    Serial.println(httpResponseCode);

    http.end();
    return (httpResponseCode == 201);
}

// Send SMS using Twilio
bool sendTwilioSMS(String message, String recipient) {
    String encodedAuth = "Basic " + base64::encode(String(twilio_account_sid) + ":" + String(twilio_auth_token));
    String postData = "From=" + String(twilio_from_number) + "&To=" + recipient + "&Body=" + URLEncode(message);

    HTTPClient http;
    http.begin("https://api.twilio.com/2010-04-01/Accounts/" + String(twilio_account_sid) + "/Messages.json");
    http.addHeader("Authorization", encodedAuth);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int httpResponseCode = http.POST(postData);
    Serial.print("Twilio SMS Response Code: ");
    Serial.println(httpResponseCode);

    http.end();
    return (httpResponseCode == 201);
}

// URL Encode Function
String URLEncode(String input) {
    String encodedString = "";
    for (int i = 0; i < input.length(); i++) {
        char c = input.charAt(i);
        if (c == ' ') encodedString += '+';
        else if (isalnum(c)) encodedString += c;
        else encodedString += '%' + String(c, HEX);
    }
    return encodedString;
}
