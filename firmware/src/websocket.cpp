#include <WebSocketsClient.h>

#include "defines.h"
#include "websocket.h"

const int WEBSOCKET_RECONNECT_INTERVAL = 5000;

const long BROADCAST_INTERVAL = 250;  // 25 milliseconds
long last_broadcast = 0;

WiFiClient client;
WebSocketsClient websocket; 

void handle_websocket_event(WStype_t type, uint8_t *payload, size_t length)
{
    switch (type) {
    case WStype_DISCONNECTED:
        Serial.println("[WSc] Disconnected!");
        break;
    case WStype_CONNECTED:
        Serial.println("[WSc] Connected!");

        // send message to server when Connected
        websocket.sendTXT("Connected");
        break;
    case WStype_TEXT:
        Serial.print("[WSc] get text:");
        Serial.println((char *)payload);

        // send message to server
        // websocket.sendTXT("message here");
        break;
    case WStype_BIN:
        // send data to server
        // websocket.sendBIN(payload, length);
        break;
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_PING:
    case WStype_PONG:
    case WStype_FRAGMENT_FIN:
        break;
    }
}

void init_websocket() {
    websocket.begin(C2_ADDR, C2_PORT);
    websocket.onEvent(handle_websocket_event);
    websocket.setReconnectInterval(WEBSOCKET_RECONNECT_INTERVAL);

    Serial.println("C2 Server Target:");
    Serial.print("Address: ");
    Serial.println(C2_ADDR);
    Serial.print("Port: ");
    Serial.println(C2_PORT);
}

void loop_websocket() {
    websocket.loop();

    const long current = millis();
    if (current - last_broadcast > BROADCAST_INTERVAL) {
        websocket.sendTXT("hello");
        last_broadcast = current;
    }
}