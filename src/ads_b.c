#include "../include/ads_b.h"

/**
 * Convert a hex string to a uint8_t array
 */
void hex_to_bytes(const char *hex_str, uint8_t *bytes, size_t len) {
  for (size_t i = 0; i < len; i++) {
    sscanf(hex_str + 2 * i, "%2hhx", &bytes[i]);
  }
}

/**
 * Function to process the ADS-B hex string
 * into adsb_ext_t or adsb_short_t
 */
void process_adsb(Logger *logger, const char *hex_str) {
  size_t len = strlen(hex_str);

  // Check if the length is either 26 or 40 hex symbols (13 or 20 bytes)
  if (len != 26 && len != 40) {
    log_error(logger, "Invalid hex string length\n");
    return;
  }

  // Allocate a byte buffer for the full message
  uint8_t full_message[20]; // Maximum size: 20 bytes (160 bits)

  // Drop the first 12 hex characters (6 bytes)
  hex_to_bytes(hex_str + 12, full_message, len / 2 - 6);

  if (len == 26) {
    // Process the 26-character hex string (13 bytes)
    // 1. Convert the next 8 hex symbols to the header
    adsb_short_t msg_short;
    msg_short.header = (full_message[0] << 24) | (full_message[1] << 16) |
                       (full_message[2] << 8) | full_message[3];
    memcpy(msg_short.parity, full_message + 4, 3);

    // Print the result
    printf("Header: 0x%08X\n", msg_short.header);
    printf("Parity: ");
    for (int i = 0; i < 3; i++) {
      printf("%02X ", msg_short.parity[i]);
    }
    printf("\n");

  } else if (len == 40) {
    // Process the 40-character hex string (20 bytes)
    // 1. Convert the next 8 hex symbols to the header
    adsb_ext_t msg_ext;
    msg_ext.header = (full_message[0] << 24) | (full_message[1] << 16) |
                     (full_message[2] << 8) | full_message[3];
    memcpy(msg_ext.data, full_message + 4, 7);
    memcpy(msg_ext.parity, full_message + 11, 3);

    // Print the result
    printf("Header: 0x%08X\n", msg_ext.header);
    printf("Data: ");
    for (int i = 0; i < 7; i++) {
      printf("%02X ", msg_ext.data[i]);
    }
    printf("\n");
    printf("Parity: ");
    for (int i = 0; i < 3; i++) {
      printf("%02X ", msg_ext.parity[i]);
    }
    printf("\n");
  }
}
