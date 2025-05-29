#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

// Fingerprint sensor setup
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// LCD setup (Change 0x27 to 0x3F if needed)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Voting button pins
#define VOTE_BTN_1      4
#define VOTE_BTN_2      8
#define VOTE_BTN_3      9
#define BUZZER_PIN      11
#define TOGGLE_SWITCH   7

// EEPROM memory locations for vote counts
#define VOTE_COUNT_1_ADDR 20
#define VOTE_COUNT_2_ADDR 30
#define VOTE_COUNT_3_ADDR 40
#define VOTE_STATUS_ADDR 50  // EEPROM Address for storing voting status per user

// Variables for vote counts
int voteCount1 = 0;
int voteCount2 = 0;
int voteCount3 = 0;

void setup() {
    pinMode(TOGGLE_SWITCH, INPUT_PULLUP);  
    pinMode(VOTE_BTN_1, INPUT_PULLUP);
    pinMode(VOTE_BTN_2, INPUT_PULLUP);
    pinMode(VOTE_BTN_3, INPUT_PULLUP);
    pinMode(BUZZER_PIN, OUTPUT);

    Serial.begin(9600);
    while (!Serial);

    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Initializing...");
    delay(2000);
    lcd.clear();

    Serial.println("\nInitializing Fingerprint Sensor...");
    finger.begin(57600);
    
    if (finger.verifyPassword()) {
        Serial.println("Fingerprint sensor found!");
        lcd.print("Sensor: OK");
    } else {
        Serial.println("Fingerprint sensor not found :(");
        lcd.print("Sensor ERROR");
        while (1) { delay(1); }
    }
    delay(2000);
    lcd.clear();

    // Load vote counts from EEPROM
    voteCount1 = EEPROM.read(VOTE_COUNT_1_ADDR);
    voteCount2 = EEPROM.read(VOTE_COUNT_2_ADDR);
    voteCount3 = EEPROM.read(VOTE_COUNT_3_ADDR);

    // Display mode based on switch position
    int state = digitalRead(TOGGLE_SWITCH);
    if (state == HIGH) {  
        Serial.println("Fingerprint Enrollment Mode");
        lcd.print("Enroll Mode");
    } else {  
        Serial.println("Voting Mode Activated");
        lcd.print("Voting Mode");
    }
    delay(2000);
    lcd.clear();
}

void loop() {
    if (digitalRead(TOGGLE_SWITCH) == HIGH) {
        reg();  // Enrollment Mode
    } else {
        votingMode();  // Voting Mode
    }
}

void votingMode() {
    lcd.clear();
    lcd.print("Voting Mode");
    displayVoteCount();  // Show vote count on LCD
    delay(1000);
    lcd.print("Scan Finger...");
    Serial.println("Waiting for fingerprint...");

    int userID = getFingerprintID();
    if (userID == -1) {
        lcd.clear();
        lcd.print("Invalid User!");
        lcd.setCursor(0, 1);
        lcd.print("Register First.");
        Serial.println("Invalid User!");
        delay(2000);
        return;
    }

    // Check if the user has already voted
    if (EEPROM.read(VOTE_STATUS_ADDR + userID) == 1) {
        lcd.clear();
        lcd.print("Already Voted!");
        Serial.println("User has already voted!");
        delay(2000);
        return;
    }

    Serial.print("User ID: "); Serial.println(userID);
    lcd.clear();
    lcd.print("User: ");
    lcd.print(userID);
    delay(1000);

    lcd.clear();
    lcd.print("Vote Now!");
    Serial.println("Fingerprint Verified. You can vote now!");

    while (true) {
        if (digitalRead(VOTE_BTN_1) == LOW) {
            voteCount1++;
            EEPROM.write(VOTE_COUNT_1_ADDR, voteCount1);
            buzz();
            lcd.clear();
            lcd.print("Voted: Option 1");
            Serial.println("Vote Registered: Option 1");
            EEPROM.write(VOTE_STATUS_ADDR + userID, 1);  // Mark user as voted
            break;
        }
        if (digitalRead(VOTE_BTN_2) == LOW) {
            voteCount2++;
            EEPROM.write(VOTE_COUNT_2_ADDR, voteCount2);
            buzz();
            lcd.clear();
            lcd.print("Voted: Option 2");
            Serial.println("Vote Registered: Option 2");
            EEPROM.write(VOTE_STATUS_ADDR + userID, 1);  // Mark user as voted
            break;
        }
        if (digitalRead(VOTE_BTN_3) == LOW) {
            voteCount3++;
            EEPROM.write(VOTE_COUNT_3_ADDR, voteCount3);
            buzz();
            lcd.clear();
            lcd.print("Voted: Option 3");
            Serial.println("Vote Registered: Option 3");
            EEPROM.write(VOTE_STATUS_ADDR + userID, 1);  // Mark user as voted
            break;
        }
    }
    
    delay(2000);
    displayVoteCount();  // Show vote count on LCD
}

void displayVoteCount() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  Voting Count");
    lcd.setCursor(0, 1);
    lcd.print("1:");
    lcd.print(voteCount1);
    lcd.print(" 2:");
    lcd.print(voteCount2);
    lcd.print(" 3:");
    lcd.print(voteCount3);
    lcd.print(" T:");
    lcd.print(voteCount1+voteCount2+voteCount3);
    delay(3000);
}

int getFingerprintID() {
    lcd.clear();
    lcd.print("Place Finger...");
    Serial.println("Place Finger...");

    while (finger.getImage() != FINGERPRINT_OK) {
        lcd.setCursor(0, 1);
        lcd.print("Scanning...");
        Serial.print(".");
        delay(500);
    }
    lcd.clear();
    lcd.print("Processing...");
    Serial.println("Processing fingerprint...");

    if (finger.image2Tz() != FINGERPRINT_OK) return -1;
    if (finger.fingerFastSearch() != FINGERPRINT_OK) return -1;
    
    lcd.clear();
    lcd.print("Fingerprint OK");
    delay(1000);
    
    return finger.fingerID;
}

void reg() {
    lcd.clear();
    lcd.print("Enroll Mode...");
    Serial.println("Ready to enroll...");

    int id;
    EEPROM.get(0, id);
    if (id < 1 || id > 127) id = 1;

    Serial.print("Enrolling ID #"); Serial.println(id);
    lcd.clear();
    lcd.print("Enrolling ID: ");
    lcd.print(id);
    delay(1000);

    int existingUser = getFingerprintID();
    if (existingUser != -1) {
        lcd.clear();
        lcd.print("Already Registered!");
        Serial.println("Fingerprint Already Registered!");
        delay(2000);
        return;
    }

    if (getFingerprintEnroll(id)) {
        Serial.println("Enroll Success!");
        lcd.clear();
        lcd.print("Enroll Success!");
        id++;
        EEPROM.put(0, id);
    } else {
        Serial.println("Enroll Failed!");
        lcd.clear();
        lcd.print("Enroll Failed!");
    }
    delay(2000);
}

uint8_t getFingerprintEnroll(int id) {
    int p = -1;
    
    lcd.clear();
    lcd.print("Place Finger...");
    
    while (p != FINGERPRINT_OK) {
        p = finger.getImage();
        if (p == FINGERPRINT_NOFINGER) {
            lcd.setCursor(0, 1);
            lcd.print("Waiting...");
            delay(200);
        }
    }
    lcd.clear();
    lcd.print("Processing...");

    p = finger.image2Tz(1);
    if (p != FINGERPRINT_OK) return false;
    
    p = finger.image2Tz(2);
    if (p != FINGERPRINT_OK) return false;
    
    p = finger.createModel();
    if (p != FINGERPRINT_OK) return false;
    
    p = finger.storeModel(id);
    return (p == FINGERPRINT_OK);
}

void buzz() {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
}
