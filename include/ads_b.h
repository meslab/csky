#ifndef ADS_B_H
#define ADS_B_H

#include "../../crux/include/logger.h"

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
} adsbExtendedMessage;

typedef struct {
  uint32_t header;   // 4 bytes (DF, CA, ICAO)
  uint8_t parity[3]; // 3 bytes (Parity)
} adsbShortMessage;

void adsb_squitter_parse(Logger *logger, const char *hex_str);

void adsb_ext_message_parse(uint8_t full_message[ADSB_EXT_LEN], Logger *logger);

void adsb_short_message_parse(uint8_t full_message[ADSB_SHORT_LEN],
                              Logger *logger);

#endif // ADS_B_H
