#ifndef WEBSOCKET_MANAGER_H
#define WEBSOCKET_MANAGER_H

#include "can.h"

void init_websocket();
void loop_websocket();

void stream_can_packet(can_packet *packet);

#endif