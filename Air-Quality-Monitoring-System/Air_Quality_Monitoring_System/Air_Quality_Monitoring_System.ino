#define BLYNK_TEMPLATE_ID "TMPL37qgZjMhk"
#define BLYNK_TEMPLATE_NAME "Air Quality Monitoring System"
#define BLYNK_AUTH_TOKEN "H3aUXVPE5LkISIFw1T8Tix-FhrtMYAiN"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define GAS_SENSOR A0   // MQ-135 Gas Sensor (Analog)
#define DHT_PIN D4      // DHT11 Temperature & Humidity Sensor
#define DHT_TYPE DHT11  // Using DHT11
#define ALERT_PIN D5    // Buzzer & LED connected here

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Gtech";     
char pass[] = "12345678";  

DHT dht(DHT_PIN, DHT_TYPE);
BlynkTimer timer;
LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD I2C Address: 0x27 (Change if needed)

int gasThreshold = 600; // Default threshold
String locationName = "Office";  // Default location name

// Read user selection from Blynk menu (V5)
BLYNK_WRITE(V4) {
    int location = param.asInt();
    switch (location) {
        case 0: gasThreshold = 500; locationName = "Home"; break;
        case 1: gasThreshold = 600; locationName = "Office"; break;
        case 2: gasThreshold = 800; locationName = "Industry"; break;
        case 3: gasThreshold = 900; locationName = "Traffic Area"; break;
        default: gasThreshold = 600; locationName = "Office"; break;
    }
    Serial.print("\n📌 Air Quality Threshold Updated: ");
    Serial.print(gasThreshold);
    Serial.print(" for ");
    Serial.println(locationName);
}

void sendSensorData() {
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    int gasValue = analogRead(GAS_SENSOR);

    // Validate sensor readings
    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("⚠️ Failed to read from DHT sensor!");
        return; 
    }

    // Print data to Serial Monitor 📟
    Serial.println("\n============================");
    Serial.println("🌍 Air Quality Monitoring System");
    Serial.println("============================");
    Serial.print("📍 Location: ");
    Serial.println(locationName);
    Serial.print("💨 Gas Level: ");
    Serial.print(gasValue);
    Serial.print(" (Threshold: ");
    Serial.print(gasThreshold);
    Serial.println(")");

    Serial.print("🌡️ Temperature: ");
    Serial.print(temperature);
    Serial.print("°C  |  💧 Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
    Serial.println("============================");

    // Send data to Blynk
    Blynk.virtualWrite(V0, gasValue);
    Blynk.virtualWrite(V1, temperature);
    Blynk.virtualWrite(V2, humidity);

    // Display data on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Gas: ");
    lcd.print(gasValue);
    lcd.print("/");
    lcd.print(gasThreshold);

    lcd.setCursor(0, 1);
    lcd.print("T:");
    lcd.print(temperature);
    lcd.print("C H:");
    lcd.print(humidity);
    lcd.print("%");

    // Check against user-defined threshold
    if (gasValue > gasThreshold) {
        Serial.println("⚠️ ALERT: High Pollution Detected! ⚠️");
        digitalWrite(ALERT_PIN, HIGH); // Turn on Buzzer & LED
        Blynk.virtualWrite(V4, 255);   // Virtual LED ON
        Blynk.logEvent("_air_quality_monitoring_system", "⚠️ High Pollution Detected! Take action!");
        
        // Alert message on LCD
        lcd.setCursor(0, 0);
        lcd.print("!! HIGH ALERT !!");
    } else {
        Serial.println("✅ Air Quality is NORMAL.");
        digitalWrite(ALERT_PIN, LOW);  // Turn off Buzzer & LED
        Blynk.virtualWrite(V4, 0);     // Virtual LED OFF
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("🚀 Starting Air Quality Monitoring System...");
    
    Blynk.begin(auth, ssid, pass);
    
    pinMode(GAS_SENSOR, INPUT);
    pinMode(ALERT_PIN, OUTPUT);
    
    dht.begin();
    lcd.init();  
    lcd.backlight(); 

    lcd.setCursor(0, 0);
    lcd.print("Air Quality Init");
    lcd.setCursor(0, 1);
    lcd.print("Loading...");
    
    delay(2000); // Show startup message
    timer.setInterval(2000L, sendSensorData);  
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("🔄 Reconnecting to WiFi...");
        Blynk.connect();
    }
    Blynk.run();
    timer.run();
}
