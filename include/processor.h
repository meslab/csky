#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "arg_parser.h"
#include "crux_logger.h"
#include "ring_buffer.h"

typedef struct {
	ringBuffer *rb;
	Options *opts;
	Logger *logger;
} ProcessorArgs;

int8_t data_processor_thread_args_init(ProcessorArgs *data_processor_args,
		ringBuffer *ring_buffer, Options *opts,
		Logger *logger);

void *data_processor_thread(void *arg);
size_t squitter_strip_chars(char *str);

#endif // PROCESSOR_H
