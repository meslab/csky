#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#include <stddef.h>
#include <stdint.h>

typedef struct Options {
  char *tcp_server;
  uint16_t tcp_port;
  uint8_t verbose;
} Options;

void parse_arguments(int argc, char *argv[], Options *opt);

#endif // ARG_PARSER_H
