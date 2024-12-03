#ifndef CAN_MANAGER_H
#define CAN_MANAGER_H

void init_can();
void can_rx_interrupt(int len);

#endif