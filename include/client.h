#ifndef CLIENT_H
#define CLIENT_H

#include "../../crux/include/logger.h"
#include "../include/arg_parser.h"
#include "../include/ring_buffer.h"

typedef struct {
  ringBuffer *rb;
  Options *opts;
  Logger *logger;
} TcpClientArgs;

int8_t tcp_client_thread_init(TcpClientArgs *tcp_client_args,
                              ringBuffer *ring_buffer, Options *opts,
                              Logger *logger);
void *tcp_client_thread(void *arg);

void lines_read(int sockfd, ringBuffer *rb);

#endif // CLIENT_H
