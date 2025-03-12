#ifndef LOGGER_H
#define LOGGER_H

#define MESSAGE_LOG_MAX_LENGTH 80

#include "../include/arg_parser.h"
#include <stdio.h>

typedef enum { DEBUG, INFO, WARNING, ERROR } LogLevel;

typedef struct {
  FILE *err_log;
  FILE *out_log;
  LogLevel level;
} Logger;

int logger_init(Logger *logger, const Options *opts);
void logger_close(Logger *Logger);

void message_log(Logger *logger, const LogLevel level, const char *message);

void error_log(Logger *logger, const char *message);
void info_log(Logger *logger, const char *message);
void warning_log(Logger *logger, const char *message);
void debug_log(Logger *logger, const char *message);

void info_log_formatted(Logger *logger, const char *format, ...);
void debug_log_formatted(Logger *logger, const char *format, ...);

#endif // LOGGER_H
