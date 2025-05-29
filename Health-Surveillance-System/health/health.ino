#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL3hHougpcA"
#define BLYNK_TEMPLATE_NAME "Dynamic Health Surveillance System"
#define BLYNK_AUTH_TOKEN "vvFRt7pJ-H2GS7Ttnk8NC8vVf8jcE8f4"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// WiFi Credentials
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "tech";
char pass[] = "12345678";

// LCD Configuration (SDA = D1, SCL = D2)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// DHT11 Configuration
#define DHTPIN D5  // GPIO2 (D5 on ESP8266)
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// DS18B20 Temperature Sensor Configuration
#define ONE_WIRE_BUS D6  // Data pin
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Pulse Sensor Configuration
#define PULSE_SENSOR_PIN A0  // Pulse Sensor connected to Analog Pin

// Buzzer and LED Configuration
#define BUZZER_PIN D7  // GPIO13
#define RED_LED_PIN D8 // GPIO15

// Reporting Time
#define REPORTING_PERIOD_MS 1000
uint32_t tsLastReport = 0;

// Variables to store sensor data
int BPM, humidity;
float roomTemp, bodyTemp;

void setup() {
    Serial.begin(115200);
    
    // Initialize I2C communication
    Wire.begin(D1, D2);
    Serial.println("I2C Initialized");

    // Initialize LCD
    lcd.init();  // Use init() instead of begin()
    lcd.backlight();
    Serial.println("LCD Initialized");

    // Startup Message
    lcd.setCursor(2, 0);
    lcd.print("Health System");
    lcd.setCursor(3, 1);
    lcd.print("Initializing...");
    delay(2000);
    lcd.clear();
    Serial.println("Startup Message Displayed");

    // Initialize Blynk
    Blynk.begin(auth, ssid, pass);
    Serial.println("Blynk Initialized");

    // Initialize Sensors
    dht.begin();
    sensors.begin();
    pinMode(PULSE_SENSOR_PIN, INPUT);
    Serial.println("Sensors Initialized");

    // Initialize Buzzer and LED
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);
    Serial.println("Buzzer and LED Initialized");
}

void loop() {
    Blynk.run();

    // Read Pulse Sensor
    int pulseValue = analogRead(PULSE_SENSOR_PIN);
    BPM = map(pulseValue, 500, 1023, 60, 120);

    // Read Humidity & Room Temperature
    humidity = dht.readHumidity();
    roomTemp = dht.readTemperature();

    // Read Body Temperature from DS18B20
    sensors.requestTemperatures();
    bodyTemp = sensors.getTempCByIndex(0);

    // Fix DS18B20 Read Error (-127.00°C)
    if (bodyTemp == -127.00) {
        Serial.println("Error: DS18B20 Not Detected! Retrying...");
        sensors.requestTemperatures();
        bodyTemp = sensors.getTempCByIndex(0);
    }

    // Update LCD only when necessary (less flickering)
    static uint32_t lastDisplayUpdate = 0;
    if (millis() - lastDisplayUpdate >= 1000) {  // Update every 1 second
        lastDisplayUpdate = millis();

        lcd.setCursor(0, 0);
        lcd.print("BPM:");
        lcd.print(BPM);

        lcd.setCursor(9, 0);
        lcd.print("BT:");
        lcd.print(bodyTemp, 1);
        lcd.print((char)223);
        lcd.print("C  ");

        lcd.setCursor(0, 1);
        lcd.print("Temp:");
        lcd.print(roomTemp, 1);
        lcd.print((char)223);
        lcd.print("C");

        lcd.setCursor(9, 1);
        lcd.print(" H:");
        lcd.print(humidity);
        lcd.print("%  ");
    }

    // Print to Serial Monitor
    Serial.print("Heart Rate: ");
    Serial.print(BPM);
    Serial.print(" BPM | Body Temp: ");
    Serial.print(bodyTemp);
    Serial.print("°C | Room Temp: ");
    Serial.print(roomTemp);
    Serial.print("°C | Humidity: ");
    Serial.print(humidity);
    Serial.println("%");

    // Send Data to Blynk
    Blynk.virtualWrite(V1, BPM);
    Blynk.virtualWrite(V2, roomTemp);
    Blynk.virtualWrite(V3, humidity);
    Blynk.virtualWrite(V4, bodyTemp);

    // Alert System: Check Abnormal Conditions
    if (BPM < 60) {
        digitalWrite(BUZZER_PIN, HIGH);
        digitalWrite(RED_LED_PIN, HIGH);
        Serial.println("ALERT: High Body Temperature or Low Heart Rate!");

        // Trigger Blynk Notification with Custom Message
        Blynk.logEvent("health_alert","🚨 Low Heart Rate 💓 Detected!");
    } 
    else if (bodyTemp > 35.0 ) {
        digitalWrite(BUZZER_PIN, HIGH);
        digitalWrite(RED_LED_PIN, HIGH);
        Serial.println("ALERT: High Body Temperature or Low Heart Rate!");

        // Trigger Blynk Notification with Custom Messagea
        Blynk.logEvent("health_alert","🚨 High Body Temp 🔥 Detected!");
    } 
    else {
        digitalWrite(BUZZER_PIN, LOW);
        digitalWrite(RED_LED_PIN, LOW);
    }

    delay(100);  // Small delay for stability
}
