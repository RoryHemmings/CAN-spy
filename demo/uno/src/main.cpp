#include <Arduino.h>
#include <Servo.h>
#include <CAN.h>

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

    if (!CAN.begin(500E3)) {
        Serial.println("CAN failed to start.");
        while (1);
    }

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
}

void can_rx_interrupt(int len) {
    Serial.print("Received Packet");

    if (CAN.packetRtr()) {
        Serial.print("RTR ");
    }

    Serial.print("packet with id 0x");
    Serial.print(CAN.packetId(), HEX);

    if (CAN.packetRtr()) {
        Serial.print(" and requested length ");
        Serial.println(CAN.packetDlc());
    } else {
        Serial.print(" and length ");
        Serial.println(len);

        // only print packet data for non-RTR packets
        while (CAN.available()) {
            Serial.print((char)CAN.read());
        }
        Serial.println();
    }

    Serial.println();
}
