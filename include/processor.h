#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "../include/arg_parser.h"
#include "../include/ring_buffer.h"

typedef struct {
  ring_buffer_t *rb;
  Options *opts;
} ProcessorArgs;

void *data_processor_thread(void *arg);
void strip_chars(char *str);

#endif // PROCESSOR_H
