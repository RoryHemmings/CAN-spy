#include <Arduino.h>

#include "can.h"
#include "defines.h"
#include "websocket.h"
#include "wifi.h"

can_packet packet_one;
can_packet packet_two;
const long fake_interval = 2000;
long last_fake = 0;

void setup()
{
    Serial.begin(9600);

#ifdef DEBUG
    while (!Serial)
        ; /* Wait till serial is ready */
#endif

    init_wifi();
    init_websocket();

    // init_can();
}

void loop()
{
    loop_websocket();

    long current = millis();
    if (current - last_fake > fake_interval) {
        packet_one.data = (byte *)"hello";
        packet_one.len = 6;
        packet_one.id = 0x12;

        packet_two.data = (byte *)"world!";
        packet_two.len = 7;
        packet_two.id = 0x14;

        stream_can_packet(&packet_one);
        stream_can_packet(&packet_two);

        last_fake = current;
    }
}