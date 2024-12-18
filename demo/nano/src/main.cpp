#include <Arduino.h>
#include <Arduino_LSM6DS3.h>
#include <CAN.h>

// #define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(__X__) Serial.print(__X__)
#define DEBUG_PRINT_FMT(__X__, __FMT__) Serial.print(__X__, __FMT__)
#define DEBUG_PRINTLN(__X__) Serial.println(__X__)
#define DEBUG_PRINTLN_FMT(__X__, __FMT__) Serial.println(__X__, __FMT__)
#else
#define DEBUG_PRINT(__X__)
#define DEBUG_PRINT_FMT(__X__, __FMT__)
#define DEBUG_PRINTLN(__X__)
#define DEBUG_PRINTLN_FMT(__X__, __FMT__)
#endif

const unsigned int SPI_CS_PIN = 10;
const unsigned int IRQ_PIN = 2;

const unsigned long SPI_FREQ = 500e3;
const unsigned long CAN_BITRATE = 500e3;
const unsigned long MPC2515_CLOCK_FREQ = 8e6;

#ifdef DEBUG
const unsigned long loop_interval = 1000; // tick every second
#else
const unsigned long loop_interval = 10; // tick every 10 milliseconds
#endif


unsigned long prev_millis = 0;
unsigned long cur_millis = 0;

const int MAX_ANGLE = 90;

void setup()
{
    Serial.begin(9600);

    #ifdef DEBUG
    while (!Serial)
        ; /* Wait till serial is ready */
    #endif

    if (!IMU.begin()) {
        DEBUG_PRINTLN("Failed to Initialize IMU");
        while (1);
    }

    // Set custom CS and Interrupt pins
    CAN.setPins(SPI_CS_PIN, IRQ_PIN);

    // Set SPI frequency to 500khz
    CAN.setSPIFrequency(SPI_FREQ);

    // Set MPC2515 Clock Frequency
    CAN.setClockFrequency(MPC2515_CLOCK_FREQ);

    // start the CAN bus at 500 kbps
    while (!CAN.begin(CAN_BITRATE)) {
        DEBUG_PRINTLN("Starting CAN failed");
        delay(1000); // Retry after 1 second
    }
}

void send_packet(float ax, float ay, float az) {
    DEBUG_PRINTLN("Sending Packet");

    int8_t angle = (int8_t) (ay*MAX_ANGLE);
    DEBUG_PRINT("Angle: ");
    DEBUG_PRINTLN(angle);

    CAN.beginPacket(0x12);
    CAN.write((byte) angle);
    CAN.endPacket();

    DEBUG_PRINTLN("Sent Packet");
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
