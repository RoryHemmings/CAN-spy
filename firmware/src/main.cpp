#include <Arduino.h>

#include "can.h"
#include "defines.h"
#include "websocket.h"
#include "wifi.h"

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
}