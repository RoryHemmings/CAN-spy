#include <Arduino.h>
#include <Arduino_LSM6DS3.h>
#include <CAN.h>

// const unsigned long loop_interval = 10; // tick every 10 milliseconds
const unsigned long loop_interval = 1000;
unsigned long prev_millis = 0;
unsigned long cur_millis = 0;

void setup()
{
    Serial.begin(9600);
    while (!Serial); /* Wait till serial is ready */

    if (!IMU.begin()) {
        Serial.println("Failed to Initialize IMU");
        while (1);
    }

    // start the CAN bus at 500 kbps
    if (!CAN.begin(500E3)) {
        Serial.println("Starting CAN failed");
        while (1);
    }
}

void loop()
{
    cur_millis = millis();

    if (cur_millis - prev_millis >= loop_interval) {
        float ax, ay, az;
        if (IMU.accelerationAvailable())
            IMU.readAcceleration(ax, ay, az);

        Serial.println("Sending Packet");

        CAN.beginPacket(0x12);
        CAN.write('h');
        CAN.write('e');
        CAN.write('l');
        CAN.write('l');
        CAN.write('o');
        CAN.endPacket();

        // CAN.beginPacket(0x12);
        // CAN.write(ax);
        // CAN.write(ay);
        // CAN.write(az);
        // CAN.endPacket();

        Serial.println("Sent Packet");

        // Serial.println(ay);
        prev_millis = cur_millis;
    }
}
