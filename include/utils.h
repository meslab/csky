#ifndef UTILS_H
#define UTILS_H
#include <stdint.h>

void hex_to_uint32(const char *hex_str, uint32_t *arr, int *count);

void to_upper_case(char *str);

#endif // UTILS_H