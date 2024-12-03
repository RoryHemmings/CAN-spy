#include "can.h"
#include <CAN.h>

const unsigned int SPI_CS_PIN = 15;
const unsigned int IRQ_PIN = 2;

const unsigned long SPI_FREQ = 500e3;
const unsigned long CAN_BITRATE = 500e3;
const unsigned long MPC2515_CLOCK_FREQ = 8e6;

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

    CAN.onReceive(can_rx_interrupt);
}

static void can_rx_interrupt(int len) {
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