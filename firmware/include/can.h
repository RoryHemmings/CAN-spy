#ifndef CAN_MANAGER_H
#define CAN_MANAGER_H

typedef struct {
    byte *data;
    int id;
    size_t len;
} can_packet;

void init_can();

#endif