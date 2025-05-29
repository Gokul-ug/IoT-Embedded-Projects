# 🗳️ Fingerprint Voting System

## 🔍 Overview
This project is a **biometric-based electronic voting system** that uses a fingerprint sensor for voter authentication. Only registered users can cast a vote, and each user is allowed to vote only once. Results are displayed on an LCD and optionally logged to serial or IoT platforms.

## 🧰 Components Used
- Arduino UNO 
- Fingerprint Sensor (R305 or GT-521F52)
- 16x2 LCD Display with I2C
- Keypad or push buttons (optional)
- Buzzer (for feedback)
- LEDs for voting confirmation
- MicroSD module (optional for vote logging)
- Servo (for ballot box or indicator)

## ⚙️ Working Principle

1. **Fingerprint Enrollment**:  
   Admin registers each user’s fingerprint and assigns a voter ID.

2. **Authentication & Voting**:  
   Voter scans fingerprint → If matched and not voted yet → Vote is accepted and stored.

3. **Result Display**:  
   Number of votes per candidate is shown on LCD or via serial.

4. **Security**:  
   - No multiple voting  
   - Only enrolled fingerprints accepted  
   - Optional: Log to SD card or Blynk IoT for remote result viewing

---

## ✅ Features
- Biometric authentication using fingerprint
- Single vote per user logic
- Real-time result display on LCD
- Feedback with buzzer/LEDs
- Secure and tamper-proof voting
- Optional integration with Blynk or SD card for live result storage

---

## 📲 Future Enhancements
- Cloud-based vote storage (e.g., Firebase)
- ESP32 + Fingerprint + IoT dashboard
- Camera-based voter photo logging
