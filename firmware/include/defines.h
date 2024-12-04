#ifndef DEFINES_H
#define DEFINES_H

#include "secrets.h"

#define DEBUG
#define AP_SSID "CAN_SPY_AP"

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