#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "../include/arg_parser.h"
#include "../include/logger.h"
#include "../include/ring_buffer.h"
#include "../include/stop.h"

typedef struct {
  ring_buffer_t *rb;
  Options *opts;
  Logger *logger;
} ProcessorArgs;

int8_t init_data_processor_args(ProcessorArgs *data_processor_args,
                                ring_buffer_t *ring_buffer, Options *opts,
                                Logger *logger);

void *data_processor_thread(void *arg);
void strip_chars(char *str);

#endif // PROCESSOR_H
