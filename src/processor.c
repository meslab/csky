#include "../include/processor.h"
#include "../include/ring_buffer.h"
#include "../include/utils.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Data processing thread: Reads from ring buffer, processes hex data
 */
void *data_processor_thread(void *arg) {
    ring_buffer_t *rb = (ring_buffer_t *)arg;
    uint32_t processed_data[8];
    int count;
  
    while (1) {
      char line[MAX_LINE_LENGTH + 1];
      if (ring_buffer_get(rb, line) == 0) {
        hex_to_uint32(line, processed_data, &count);
        printf("Processed data: ");
        for (int i = 0; i < count; i++) {
          printf("%X ", processed_data[i]);
        }
        printf("\n");
      }
    }
    return NULL;
  }
  