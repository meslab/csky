#ifndef ADS_B_H
#define ADS_B_H

#include "../include/logger.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADSB_EXT_LEN 14
#define ADSB_SHORT_LEN 7

typedef struct {
  uint32_t header;   // 4 bytes (DF, CA, ICAO)
  uint8_t data[7];   // 7 bytes (ADS-B payload)
  uint8_t parity[3]; // 3 bytes (Parity)
} adsb_ext_t;

typedef struct {
  uint32_t header;   // 4 bytes (DF, CA, ICAO)
  uint8_t parity[3]; // 3 bytes (Parity)
} adsb_short_t;

void process_adsb(Logger *logger, const char *hex_str);

void process_ext_message(uint8_t full_message[ADSB_EXT_LEN], Logger *logger);

void process_short_message(uint8_t full_message[ADSB_SHORT_LEN], Logger *logger);

#endif // ADS_B_H
