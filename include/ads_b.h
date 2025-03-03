#ifndef ADS_B_H
#define ADS_B_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  uint32_t header;   // 4 bytes (DF, CA, ICAO)
  uint8_t data[7];   // 7 bytes (ADS-B payload)
  uint8_t parity[3]; // 3 bytes (Parity)
} adsb_ext_t;

typedef struct {
  uint32_t header;   // 4 bytes (DF, CA, ICAO)
  uint8_t parity[3]; // 3 bytes (Parity)
} adsb_short_t;

void process_adsb(const char *hex_str);

#endif // ADS_B_H
