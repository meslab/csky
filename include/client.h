#ifndef CLIENT_H
#define CLIENT_H

#include "../include/arg_parser.h"
#include "../include/logger.h"
#include "../include/ring_buffer.h"
#include "../include/stop.h"

typedef struct {
  ring_buffer_t *rb;
  Options *opts;
  Logger *logger;
} TcpClientArgs;

int8_t init_tcp_client_args(TcpClientArgs *tcp_client_args,
                            ring_buffer_t *ring_buffer, Options *opts,
                            Logger *logger);
void *tcp_client_thread(void *arg);

void read_lines(int sockfd, ring_buffer_t *rb);

#endif // CLIENT_H