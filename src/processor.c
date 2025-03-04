#include "../include/processor.h"
#include "../include/ads_b.h"
#include "../include/logger.h"
#include "../include/ring_buffer.h"
#include "../include/utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Data processing thread: Reads from ring buffer, processes hex data
 */
void *data_processor_thread(void *arg) {
  ProcessorArgs *args = (ProcessorArgs *)arg;

  // Options *opts = args->opts;
  ring_buffer_t *rb = args->rb;

  while (1) {
    char line[MAX_LINE_LENGTH + 1];
    if (ring_buffer_get(rb, line) == 0) {
      strip_chars(line);
      log_info(line);
      process_adsb(line);
    }
  }
  return NULL;
}

void strip_chars(char *str) {
  int i, j = 0;
  int len = strlen(str);

  for (i = 0; i < len; i++) {
    if (str[i] != '@' && str[i] != ';') {
      str[j++] = str[i];
    }
  }
  str[j] = '\0';
}