#include <WebSocketsClient.h>
#include <cppQueue.h>

#include "can.h"
#include "defines.h"
#include "websocket.h"

const int WEBSOCKET_RECONNECT_INTERVAL = 1000;

const long BROADCAST_INTERVAL = 10; // milliseconds
long last_broadcast = 0;

cppQueue can_packet_queue(sizeof(can_packet), MAX_CAN_PACKET_BACKUP);

/**
 * Static circular buffer to store can_packet data between broadcasts.
 * Circumvents dynamic memory allocation.
 */
byte can_packet_buffers[MAX_CAN_PACKET_BACKUP][MAX_CAN_PACKET_SIZE];
can_packet can_packets[MAX_CAN_PACKET_BACKUP];
int next_can_packet = 0;

WiFiClient client;
WebSocketsClient websocket;

static void broadcast_available();
static can_packet *copy_can_packet(const can_packet *packet);
static bool deserialize_can_packet(const byte *buf, can_packet &packet);
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

void stream_can_packet(const can_packet *packet)
{
    if (can_packet_queue.isFull()) {
        return;
    }

    // Pulls copy from temporary storage buffer
    can_packet *packet_copy = copy_can_packet(packet);
    can_packet_queue.push(packet_copy);
}

static void broadcast_available()
{
    // Drain queue of can_packets
    can_packet packet;
    while (can_packet_queue.pop(&packet)) {
        if (!websocket.isConnected()) {
            continue;
        }

        // DEBUG_PRINT("Broadcasting can packet with id: ");
        // DEBUG_PRINT(packet.id);
        // DEBUG_PRINT(" and len: ");
        // DEBUG_PRINTLN(packet.len);

        // Serialize can_packet struct into byte array
        size_t buf_len = sizeof(can_packet) - sizeof(byte *) + (packet.len * sizeof(byte));
        byte buf[buf_len];

        int marker = 0;
        memcpy(buf, &packet.timestamp, sizeof(unsigned long));
        marker += sizeof(unsigned long);

        memcpy(buf + marker, &packet.id, sizeof(long));
        marker += sizeof(long);

        memcpy(buf + marker, &packet.len, sizeof(size_t));
        marker += sizeof(size_t);

        memcpy(buf + marker, packet.data, packet.len * sizeof(byte));

        websocket.sendBIN(buf, buf_len);
    }
}

/**
 * Note: does not allocate space for underlying can data
 */
static bool deserialize_can_packet(const byte *buf, int buf_len, can_packet &packet)
{
    int marker = 0;
    memcpy(&packet.timestamp, buf, sizeof(unsigned long));
    marker += sizeof(unsigned long);

    memcpy(&packet.id, buf + marker, sizeof(long));
    marker += sizeof(long);

    memcpy(&packet.len, buf + marker, sizeof(size_t));
    marker += sizeof(size_t);

    if (marker + packet.len > buf_len) {
        return false;
    }

    memcpy(packet.data, buf + marker, packet.len);
    return true;
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
        byte data[MAX_CAN_PACKET_SIZE];
        can_packet packet;
        packet.data = data;
        deserialize_can_packet(payload, length, packet);

        // DEBUG_PRINTLN((int) packet.data[0]);
        // DEBUG_PRINTLN(packet.timestamp);
        // DEBUG_PRINTLN(packet.id);
        // DEBUG_PRINTLN(packet.len);

        write_can_packet(&packet);
        break;
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_PING:
        // Respond to keep-alive ping
        websocket.sendPing();
        break;
    case WStype_PONG:
    case WStype_FRAGMENT_FIN:
        break;
    }
}

/**
 * Creates copy of a given can_packet struct with copy memory pulled
 * from in-between broadcast buffer.
 */
static can_packet *copy_can_packet(const can_packet *packet)
{
    if (next_can_packet >= MAX_CAN_PACKET_BACKUP) {
        next_can_packet = 0;
    }

    can_packet *new_packet = &(can_packets[next_can_packet]);
    byte *buf = can_packet_buffers[next_can_packet++];
    memcpy(buf, packet->data, packet->len);

    new_packet->data = buf;
    new_packet->len = packet->len;
    new_packet->id = packet->id;
    new_packet->timestamp = packet->timestamp;

    return new_packet;
}