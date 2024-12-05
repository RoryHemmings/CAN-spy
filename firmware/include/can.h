#ifndef CAN_MANAGER_H
#define CAN_MANAGER_H

const unsigned long MAX_CAN_PACKET_SIZE = 8;

typedef struct {
    byte *data;
    long id;
    size_t len;
} can_packet;

void init_can();

#endif