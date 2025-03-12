#include "../include/ads_b.h"

/// @brief Convert a hex string to a byte array
/// @param hex_str
/// @param bytes
/// @param len
void hex_to_bytes(const char *hex_str, uint8_t *bytes, size_t len) {
  for (size_t i = 0; i < len; i++) {
    sscanf(hex_str + 2 * i, "%2hhx", &bytes[i]);
  }
}

/// @brief Process the ADS-B hex string
/// @param logger
/// @param hex_str  The hex string to process
void adsb_squitter_parse(Logger *logger, const char *hex_str) {
  size_t len = strlen(hex_str);

  uint8_t short_message[7]; // Maximum size: 7 bytes (56 bits)
  uint8_t full_message[14]; // Maximum size: 14 bytes (112 bits)

  switch (len) {
  case 26:
    hex_to_bytes(hex_str + 12, short_message, len / 2 - 6);
    adsb_short_message_parse(short_message, logger);
    break;
  case 40:
    hex_to_bytes(hex_str + 12, full_message, len / 2 - 6);
    adsb_ext_message_parse(full_message, logger);
    break;
  default:
    error_log(logger, "Invalid hex string length\n");
    break;
  }
}

/// @brief Process an extended message
/// @param full_message
/// @param logger
void adsb_ext_message_parse(uint8_t full_message[ADSB_EXT_LEN],
                            Logger *logger) {
  adsbExtendedMessage msg_ext;
  msg_ext.header = (full_message[0] << 24) | (full_message[1] << 16) |
                   (full_message[2] << 8) | full_message[3];
  memcpy(msg_ext.data, full_message + 4, 7);
  memcpy(msg_ext.parity, full_message + 11, 3);

  char data_str[3 * 7];   // Each byte: "XX " (3 chars), 7 bytes -> 21 chars
  char parity_str[3 * 3]; // Each byte: "XX " (3 chars), 3 bytes -> 9 chars

  snprintf(data_str, sizeof(data_str), "%02X %02X %02X %02X %02X %02X %02X",
           msg_ext.data[0], msg_ext.data[1], msg_ext.data[2], msg_ext.data[3],
           msg_ext.data[4], msg_ext.data[5], msg_ext.data[6]);

  snprintf(parity_str, sizeof(parity_str), "%02X %02X %02X", msg_ext.parity[0],
           msg_ext.parity[1], msg_ext.parity[2]);

  info_log_formatted(logger, "\nHeader: 0x%08X\nData: %s\nParity: %s",
                     msg_ext.header, data_str, parity_str);
}

/// @brief Process a short message
/// @param full_message
/// @param logger
void adsb_short_message_parse(uint8_t full_message[ADSB_SHORT_LEN],
                              Logger *logger) {
  adsbShortMessage msg_short;
  msg_short.header = (full_message[0] << 24) | (full_message[1] << 16) |
                     (full_message[2] << 8) | full_message[3];
  memcpy(msg_short.parity, full_message + 4, 3);

  char parity_str[3 * 3]; // Enough for "XX XX XX\0"
  snprintf(parity_str, sizeof(parity_str), "%02X %02X %02X",
           msg_short.parity[0], msg_short.parity[1], msg_short.parity[2]);

  info_log_formatted(logger, "\nHeader: 0x%08X\nParity: %s", msg_short.header,
                     parity_str);
}
