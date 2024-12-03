#include <Arduino.h>
#include <Servo.h>
#include <CAN.h>

#define DEBUG

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

const unsigned long loop_interval = 1;
unsigned long prev_millis = 0;
unsigned long cur_millis = 0;

Servo servo;
const unsigned int servo_pin = 8;
const double servo_speed = 1.0; // one degree per tick
double servo_angle = 0;
double servo_target = 0;

double clamp_angle(double angle) {
    return min(max(0, angle), 180);
}

void set_servo_target(double target) {
    servo_target = clamp_angle(target);
}

void update_servo() {
    if (servo_angle == servo_target) {
        return;
    }

    double delta = (servo_angle < servo_target) ? servo_speed : -servo_speed;
    servo_angle = clamp_angle(servo_angle + delta);
    servo.write(servo_angle);
}

void parse_input_buffer(int *buf, int len) {
    if (len < 1) {
        DEBUG_PRINTLN("Invalid Input Buffer!");
        return;
    }

    int angle = (int) ((int8_t) buf[0]);
    DEBUG_PRINTLN((double) angle);
    set_servo_target((double) angle);
}

void can_rx_interrupt(int len) {
    // DEBUG_PRINT("Received Packet ");

    if (CAN.packetRtr()) {
        // DEBUG_PRINT("RTR ");
    }

    // DEBUG_PRINT("id 0x");
    // DEBUG_PRINT_FMT(CAN.packetId(), HEX);

    if (CAN.packetRtr()) {
        // DEBUG_PRINT(" requested length ");
        // DEBUG_PRINTLN(CAN.packetDlc());
    } else {
        // DEBUG_PRINT(" length ");
        // DEBUG_PRINTLN(len);

        // only print packet data for non-RTR packets
        int buf[len];
        for (int i = 0; i < len && CAN.available(); i++) {
            buf[i] = CAN.read();
        }

        parse_input_buffer(buf, len);
        // DEBUG_PRINTLN();
    }

    // DEBUG_PRINTLN();
}

void setup()
{
    servo.attach(servo_pin);

    Serial.begin(9600);
    while (!Serial); /* Wait till serial is ready */

    // Set custom CS and Interrupt pin
    CAN.setPins(SPI_CS_PIN, IRQ_PIN);

    // Set SPI Frequency
    CAN.setSPIFrequency(SPI_FREQ); 

    // Set MPC2515 Clock Frequency
    CAN.setClockFrequency(MPC2515_CLOCK_FREQ);

    while (!CAN.begin(CAN_BITRATE)) {
        DEBUG_PRINTLN("failed to start CAN.");
        delay(1000); // Retry after 1 second
    }

    DEBUG_PRINTLN("started CAN.");

    CAN.onReceive(can_rx_interrupt);
}

void loop()
{
    cur_millis = millis();

    if (cur_millis - prev_millis >= loop_interval) {
        update_servo();
        prev_millis = cur_millis;
    }
}