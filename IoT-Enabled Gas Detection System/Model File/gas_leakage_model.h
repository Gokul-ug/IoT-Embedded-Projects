#ifndef GAS_LEAKAGE_MODEL_H
#define GAS_LEAKAGE_MODEL_H

#include <Arduino.h>

// Function to determine leakage probability
int getLeakageProbability(int sensorValue) {
    if (sensorValue < 200) return random(0, 4);       // 0-3%
    else if (sensorValue < 400) return random(3, 10); // 3-10%
    else if (sensorValue < 500) return random(10, 22);
    else if (sensorValue < 600) return random(20, 42);
    else if (sensorValue < 700) return random(35, 55);
    else if (sensorValue < 800) return random(50, 72);
    else if (sensorValue < 1000) return random(75, 85);
    else if (sensorValue < 2000) return random(89, 92);
    else if (sensorValue < 2500) return random(92, 94);
    else if (sensorValue < 3000) return random(94, 96);
    else if (sensorValue < 3500) return random(95, 97);
    else if (sensorValue < 4000) return random(97, 99);
    else return 100; // Above 4000 is 100% leakage
}

#endif // GAS_LEAKAGE_MODEL_H
