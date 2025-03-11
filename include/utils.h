#ifndef UTILS_H
#define UTILS_H
#include <stddef.h>
#include <stdint.h>

int hex_to_uint32(const char *hex_str, uint32_t *arr, int *count,
                  size_t max_count);

int to_upper_case(char *str);

#endif // UTILS_H