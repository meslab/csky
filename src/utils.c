#include "../include/utils.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Convert a hex string to an array of uint32_t values
void hex_to_uint32(const char *hex_str, uint32_t *arr, int *count) {
  *count = 0;
  for (size_t i = 0; i < strlen(hex_str) && i + 8 <= strlen(hex_str); i += 8) {
    char chunk[9] = {0};
    strncpy(chunk, hex_str + i, 8);
    arr[(*count)++] = strtoul(chunk, NULL, 16);
  }
}

void to_upper_case(char *str) {
  while (*str) {
    *str = toupper(*str);
    str++;
  }
}
