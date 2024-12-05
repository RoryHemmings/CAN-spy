#ifndef DEFINES_H
#define DEFINES_H

#include "secrets.h"

// Maximum of can packets allowed in broadcast queue between broadcasts
const uint16_t MAX_CAN_PACKET_BACKUP = 100;

#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(__X__) Serial.print(__X__)
#define DEBUG_PRINT_FMT(__X__, __FMT__) Serial.print(__X__, __FMT__)
#define DEBUG_PRINTLN(__X__) Serial.println(__X__)
#define DEBUG_PRINTLN_FMT(__X__, __FMT__) Serial.println(__X__, __FMT__)
#else
#define DEBUG_PRINT(__X__)
#define DEBUG_PRINT_FMT(__X__, __FMT__)
#define DEBUG_PRINTLN(__X__)
#define DEBUG_PRINTLN_FMT(__X__, __FMT__)
#endif

#endif