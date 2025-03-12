#include "../include/processor.h"
#include "../include/ads_b.h"
#include "../include/logger.h"
#include "../include/ring_buffer.h"
#include "../include/utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/// @brief Initialize data processor arguments
/// @param data_processor_args Data processor arguments
/// @param ring_buffer Ring buffer
/// @param opts Options
/// @param logger Logger
/// @return int8_t 0 on success, -1 on failure
inline int8_t
data_processor_thread_args_init(ProcessorArgs *data_processor_args,
                                ring_buffer_t *ring_buffer, Options *opts,
                                Logger *logger) {
  if (!data_processor_args) {
    return -1;
  }
  data_processor_args->rb = ring_buffer;
  data_processor_args->opts = opts;
  data_processor_args->logger = logger;

  return 0;
}

/// @brief Data processor thread
/// @param arg Processor arguments
/// @return void*
void *data_processor_thread(void *arg) {
  ProcessorArgs *args = (ProcessorArgs *)arg;

  ring_buffer_t *rb = args->rb;
  Logger *logger = args->logger;

  while (1) {
    char line[MAX_LINE_LENGTH + 1];
    if (ring_buffer_get(rb, line) == 0) {
      squitter_strip_chars(line);
      debug_log(logger, line);
      debug_log_formatted(
          logger, "Head: %2d, tail: %2d, entries: %2d", rb->head, rb->tail,
          rb->head < rb->tail ? rb->head - rb->tail + BUFFER_SIZE
                              : rb->head - rb->tail);
      adsb_squitter_parse(logger, line);
    }
  }
  return NULL;
}

/// @brief Strip characters from a string
/// @param str String to strip characters from (in-place)
/// @return void
size_t squitter_strip_chars(char *str) {
  int i, j = 0;
  int len = strlen(str);

  for (i = 0; i < len; i++) {
    if (str[i] != '@' && str[i] != ';') {
      str[j++] = str[i];
    }
  }
  str[j] = '\0';
  return j;
}
