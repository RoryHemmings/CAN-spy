#include <Arduino.h>
#include <Arduino_LSM6DS3.h>
#include <CAN.h>

const unsigned long SPI_FREQ = 500e3;
const unsigned long CAN_BITRATE = 500e3;
const unsigned long MPC2515_CLOCK_FREQ = 8e6;

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

    // Set SPI frequency to 500khz
    CAN.setSPIFrequency(SPI_FREQ);

    // Set MPC2515 Clock Frequency
    CAN.setClockFrequency(MPC2515_CLOCK_FREQ);

    // start the CAN bus at 500 kbps
    while (!CAN.begin(CAN_BITRATE)) {
        Serial.println("Starting CAN failed");
        delay(1000);
    }
}

void send_packet(float ax, float ay, float az) {
    Serial.println("Sending Packet");

    CAN.beginPacket(0x12);
    CAN.write(ax);
    CAN.write(ay);
    CAN.write(az);
    CAN.endPacket();

    Serial.println("Sent Packet");
}

void loop()
{
    cur_millis = millis();

    if (cur_millis - prev_millis >= loop_interval) {
        float ax, ay, az;
        if (IMU.accelerationAvailable())
            IMU.readAcceleration(ax, ay, az);

        send_packet(ax, ay, az);
        prev_millis = cur_millis;
    }
}
