#include <CAN.h>

#include "can.h"
#include "defines.h"
#include "websocket.h"

const unsigned int SPI_CS_PIN = 10;
const unsigned int IRQ_PIN = 2;

const unsigned long SPI_FREQ = 500e3;
const unsigned long CAN_BITRATE = 500e3;
const unsigned long MPC2515_CLOCK_FREQ = 8e6;

const unsigned long MAX_CAN_PACKET_SIZE = 8;
byte recv_buffer[MAX_CAN_PACKET_SIZE];

static void can_rx_interrupt(int len) {
    DEBUG_PRINT("Received Packet ");

    if (CAN.packetRtr()) {
        // DEBUG_PRINT("RTR ");
    }

    can_packet packet;
    packet.id = CAN.packetId();
    packet.len = len;
    packet.data = recv_buffer;

    for (int i = 0; CAN.available() && i < len && i < MAX_CAN_PACKET_SIZE; i++) {
        recv_buffer[i] = (byte) CAN.read();
    }

    // TODO create static deep copies using queue
    stream_can_packet(&packet);

    // DEBUG_PRINT("id 0x");
    // DEBUG_PRINT_FMT(CAN.packetId(), HEX);

    if (CAN.packetRtr()) {
        // DEBUG_PRINT(" requested length ");
        // DEBUG_PRINTLN(CAN.packetDlc());
    } else {
        // DEBUG_PRINT(" length ");
        // DEBUG_PRINTLN(len);

        // only print packet data for non-RTR packets
        // DEBUG_PRINTLN();
    }

    // DEBUG_PRINTLN();
}

void init_can()
{
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

    DEBUG_PRINTLN("Started CAN");

    // interrupts not supported by esp8266
    CAN.onReceive(can_rx_interrupt);
}