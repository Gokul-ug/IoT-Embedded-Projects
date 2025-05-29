#include <EEPROM.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

// Define the fingerprint sensor serial communication pins
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
    Serial.begin(9600);
    Serial.println("Starting Reset...");

    // Reset EEPROM
    Serial.println("Resetting EEPROM...");
    for (int i = 0; i < EEPROM.length(); i++) {
        EEPROM.write(i, 0);  // Writing 0xFF (default blank value)
    }
    Serial.println("EEPROM Reset Complete!");

    // Initialize fingerprint sensor
    finger.begin(57600);
    if (finger.verifyPassword()) {
        Serial.println("Fingerprint sensor detected!");
    } else {
        Serial.println("Fingerprint sensor not found!");
        while (1);  // Halt if the sensor is not found
    }

    // Delete all fingerprints
    Serial.println("Deleting all stored fingerprints...");
    deleteAllFingerprints();

    Serial.println("All fingerprints erased successfully!");
    Serial.println("Reset Complete!");
}

void loop() {
    // Nothing to do here
}

// Function to delete all stored fingerprints
void deleteAllFingerprints() {
    uint8_t p = finger.emptyDatabase();
    if (p == FINGERPRINT_OK) {
        Serial.println("Fingerprint database cleared successfully!");
    } else {
        Serial.println("Error: Failed to clear fingerprint database!");
    }
}
