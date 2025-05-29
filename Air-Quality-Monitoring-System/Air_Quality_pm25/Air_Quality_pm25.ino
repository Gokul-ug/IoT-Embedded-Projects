#define BLYNK_TEMPLATE_ID "TMPL37qgZjMhk"
#define BLYNK_TEMPLATE_NAME "Air Quality Monitoring System"
#define BLYNK_AUTH_TOKEN "H3aUXVPE5LkISIFw1T8Tix-FhrtMYAiN"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ----- Pins -----
#define GAS_SENSOR A0       // MQ-135
#define DHT_PIN D4          // DHT11 Sensor
#define DHT_TYPE DHT11
#define ALERT_PIN D5        // Buzzer + LED
#define dustPin A0          // GP2Y1010AU0F output → Analog pin (same as GAS_SENSOR)
#define ledPin D6           // GP2Y1010AU0F IR LED Control Pin

// ----- Auth & WiFi -----
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Gtech";
char pass[] = "12345678";

// ----- Globals -----
DHT dht(DHT_PIN, DHT_TYPE);
BlynkTimer timer;
LiquidCrystal_I2C lcd(0x27, 16, 2);

int gasThreshold = 600;
String locationName = "Home";

// ----- BLYNK MENU: V4 -----
BLYNK_WRITE(V4) {
  int location = param.asInt();
  switch (location) {
    case 0: gasThreshold = 600; locationName = "Home"; break;
    case 1: gasThreshold = 700; locationName = "Office"; break;
    case 2: gasThreshold = 800; locationName = "Industry"; break;
    case 3: gasThreshold = 900; locationName = "Traffic Area"; break;
    default: gasThreshold = 600; locationName = "Office"; break;
  }
  Serial.print("\n Air Quality Threshold Updated: ");
  Serial.print(gasThreshold);
  Serial.print(" for ");
  Serial.println(locationName);
}

// ----- SENSOR READ + DISPLAY -----
void sendSensorData() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int gasValue = analogRead(GAS_SENSOR);

  // Dust Sensor Reading with D6 pulse
  digitalWrite(ledPin, LOW);           // Turn ON IR LED
  delayMicroseconds(280);             
  int rawDust = analogRead(dustPin);   // Read analog value
  delayMicroseconds(40);
  digitalWrite(ledPin, HIGH);          // Turn OFF IR LED
  delayMicroseconds(9680);
  float voltage = rawDust * (3.3 / 1024.0);
  float dustDensity = 0.17 * voltage - 0.1;
  if (dustDensity < 0) dustDensity = 0.00;

  // Serial Output 📟
  Serial.println("\n============================");
  Serial.println("🌍 Air Quality Monitoring System");
  Serial.println("============================");
  Serial.print("📍 Location: "); Serial.println(locationName);
  Serial.print("💨 Gas Level: "); Serial.print(gasValue);
  Serial.print(" (Threshold: "); Serial.print(gasThreshold); Serial.println(")");
  Serial.print("🌡️ Temp: "); Serial.print(temperature); Serial.print("°C  |  💧 Humidity: ");
  Serial.print(humidity); Serial.println("%");
  Serial.print("🌫️ Dust Voltage: "); Serial.print(voltage); Serial.println(" V");
  Serial.print("🌫️ Dust Density: "); Serial.print(dustDensity, 2); Serial.println(" mg/m³");
  Serial.println("============================");

  // Blynk Virtual Pins
  Blynk.virtualWrite(V0, gasValue);
  Blynk.virtualWrite(V1, temperature);
  Blynk.virtualWrite(V2, humidity);
  Blynk.virtualWrite(V5, dustDensity);  // V5: Dust Density

  // LCD Display
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gas:");
  lcd.print(gasValue);
  lcd.print("/");
  lcd.print(gasThreshold);

  lcd.setCursor(0, 1);
  lcd.print("Dust:");
  lcd.print(dustDensity, 2); // Display 2 decimal places

  // Alert System
  if (gasValue > gasThreshold) {
    Serial.println("⚠️ ALERT: High Pollution Detected!");
    digitalWrite(ALERT_PIN, HIGH);
    Blynk.virtualWrite(V3, 255); // LED ON in Blynk
    Blynk.logEvent("_air_quality_monitoring_system", "⚠️ High Pollution Detected! Take action!");
    
    lcd.setCursor(0, 0);
    lcd.print("!! HIGH ALERT !!");
  } else {
    Serial.println("✅ Air Quality is NORMAL.");
    digitalWrite(ALERT_PIN, LOW);
    Blynk.virtualWrite(V3, 0); // LED OFF in Blynk
  }
}

// ----- SETUP -----
void setup() {
  Serial.begin(115200);
  Serial.println("🚀 Starting Air Quality Monitoring System...");

  Blynk.begin(auth, ssid, pass);
  pinMode(GAS_SENSOR, INPUT);
  pinMode(ALERT_PIN, OUTPUT);
  pinMode(ledPin, OUTPUT); // D6 as Dust Sensor IR LED
  digitalWrite(ledPin, HIGH); // Make sure LED is OFF initially

  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); lcd.print("Air Quality Init");
  lcd.setCursor(0, 1); lcd.print("Loading...");
  delay(2000);

  timer.setInterval(2000L, sendSensorData);
}

// ----- LOOP -----
void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("🔄 Reconnecting to WiFi...");
    Blynk.connect();
  }
  Blynk.run();
  timer.run();
}
