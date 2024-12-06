#ifndef CAN_MANAGER_H
#define CAN_MANAGER_H

const unsigned long MAX_CAN_PACKET_SIZE = 8;

/**
 * Note: timestamps is microseconds since beginning of program. 
 * use only for packet id purposes.
 * 
 * Also, timestamps reset after 70 minutes, so don't expect it
 * to be unique if examining packet sequences of longer than 70
 * minutes.
 * 
 * Also, id refers to id on the CAN BUS, not a unique identifier.
 */
typedef struct {
    byte *data;
    unsigned long timestamp;
    long id;
    size_t len;
} can_packet;

void init_can();
void write_can_packet(const can_packet *packet);

#endif