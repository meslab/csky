#include "../include/processor.h"
#include "../include/ads_b.h"
#include "../include/logger.h"
#include "../include/ring_buffer.h"
#include "../include/utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Init ProcessorArgs
 */
inline int8_t init_data_processor_args(ProcessorArgs *data_processor_args,
                                       ring_buffer_t *ring_buffer,
                                       Options *opts, Logger *logger) {
  if (!data_processor_args) {
    return -1;
  }
  data_processor_args->rb = ring_buffer;
  data_processor_args->opts = opts;
  data_processor_args->logger = logger;

  return 0;
}

/**
 * Data processing thread: Reads from ring buffer, processes hex data
 */
void *data_processor_thread(void *arg) {
  ProcessorArgs *args = (ProcessorArgs *)arg;

  ring_buffer_t *rb = args->rb;
  Logger *logger = args->logger;

  while (1) {
    char line[MAX_LINE_LENGTH + 1];
    if (ring_buffer_get(rb, line) == 0) {
      strip_chars(line);
      log_info(logger, line);
      log_info_formatted(logger, "Head: %d, tail: %d\n", rb->head, rb->tail);
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