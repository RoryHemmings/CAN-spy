#ifndef WEBSOCKET_MANAGER_H
#define WEBSOCKET_MANAGER_H

#include "can.h"

void init_websocket();
void loop_websocket();

/**
 * Stores queues a static copy of packet for streaming 
 * upon next broadcast interval
 */
void stream_can_packet(const can_packet *packet);

#endif