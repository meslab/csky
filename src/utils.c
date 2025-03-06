#include "../include/utils.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/// @brief  Convert a hex string to a uint32_t array
/// @param hex_str  The hex string
/// @param arr      The array to store the uint32_t values
/// @param count    The number of uint32_t values
void hex_to_uint32(const char *hex_str, uint32_t *arr, int *count) {
  *count = 0;
  for (size_t i = 0; i < strlen(hex_str) && i + 8 <= strlen(hex_str); i += 8) {
    char chunk[9] = {0};
    strncpy(chunk, hex_str + i, 8);
    arr[(*count)++] = strtoul(chunk, NULL, 16);
  }
}

/// @brief  Convert a uint32_t array to a hex string
/// @param str    The array of uint32_t values
void to_upper_case(char *str) {
  while (*str) {
    *str = toupper(*str);
    str++;
  }
}
