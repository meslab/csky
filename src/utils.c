#include "../include/utils.h"

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/// @brief  Convert a hex string to a uint32_t array
/// @param hex_str  The hex string
/// @param arr      The array to store the uint32_t values
/// @param count    The number of uint32_t values
int hex_to_uint32(const char *hex_str, uint32_t *arr, int *count,
                  size_t max_count) {
  if (!hex_str || !arr || !count) {
    return -1; // Error: NULL pointer
  }

  size_t len = strlen(hex_str);
  *count = 0;

  for (size_t i = 0; i + 8 <= len && *count < (int)max_count; i += 8) {
    char chunk[9] = {0};           // 8 hex digits + null terminator
    memcpy(chunk, hex_str + i, 8); // Avoids strncpy's overhead

    char *endptr;
    errno = 0;
    uint32_t value = (uint32_t)strtoul(chunk, &endptr, 16);

    if (errno == ERANGE || endptr == chunk) {
      return -2; // Error: Conversion failure
    }

    arr[(*count)++] = value;
  }

  return 0; // Success
}

/// @brief  Convert a uint32_t array to a hex string
/// @param str    The array of uint32_t values
int to_upper_case(char *str) {
  if (str == NULL) {
    return -1; // Error: NULL pointer
  }

  for (; *str; ++str) {
    *str = (char)toupper((unsigned char)*str);
  }

  return 0; // Success
}
