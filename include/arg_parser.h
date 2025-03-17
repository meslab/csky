#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#include <stddef.h>
#include <stdint.h>

typedef struct Options {
	uint16_t tcp_port;
	uint8_t verbose;
	char *tcp_server;
	char *log_level;
	char *err_log;
	char *out_log;
} Options;

void arguments_parse(int argc, char *argv[], Options *opt);

#endif // ARG_PARSER_H
