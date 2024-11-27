#include <Arduino.h>
#include <Servo.h>
#include <CAN.h>

const unsigned long SPI_CS_PIN = 10;
const unsigned long IRQ_PIN = 2;

const unsigned long SPI_FREQ = 500e3;
const unsigned long CAN_BITRATE = 500e3;
const unsigned long MPC2515_CLOCK_FREQ = 8e6;

const unsigned long loop_interval = 10; // tick every 10 milliseconds
unsigned long prev_millis = 0;
unsigned long cur_millis = 0;

Servo servo;
const unsigned int servo_pin = 8;
const double servo_speed = 1.0; // one degree per tick
double servo_angle = 0;
double servo_target = 0;

const unsigned long rand_loop_interval = 1000;
unsigned long rand_prev_millis = 0;
unsigned long rand_cur_millis = 0;

void can_rx_interrupt(int len);

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
        Serial.println("failed to start CAN.");
        delay(1000); // Retry after 1 second
        // while (1);
    }

    Serial.println("started CAN.");

    CAN.onReceive(can_rx_interrupt);
}

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

void loop()
{
    cur_millis = millis();

    if (cur_millis - prev_millis >= loop_interval) {
        update_servo();
        prev_millis = cur_millis;
    }

    rand_cur_millis = millis();
    if (rand_cur_millis - rand_prev_millis >= rand_loop_interval) {
        set_servo_target(random(0, 180));
        rand_prev_millis = rand_cur_millis;
    }

    // int size = CAN.parsePacket();
    // Serial.println(size);
}

void can_rx_interrupt(int len) {
    Serial.print("Received Packet ");

    if (CAN.packetRtr()) {
        Serial.print("RTR ");
    }

    Serial.print("id 0x");
    Serial.print(CAN.packetId(), HEX);

    if (CAN.packetRtr()) {
        Serial.print(" requested length ");
        Serial.println(CAN.packetDlc());
    } else {
        Serial.print(" length ");
        Serial.println(len);

        // only print packet data for non-RTR packets
        float buf[3];
        while (CAN.available()) {
            // if (available >= 3) {
            //     buf[0] = CAN.read();
            //     buf[1] = CAN.read();
            //     buf[2] = CAN.read();
            // }
            Serial.println((float)CAN.read());
        }
        // Serial.println(buf[2]);
        Serial.println();
    }

    Serial.println();
}
