#ifndef LOGGER_H
#define LOGGER_H

#define LOG_MESSAGE_MAX_LENGTH 80

#include "../include/arg_parser.h"
#include <stdio.h>

typedef enum { LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR } LogLevel;

typedef struct {
  FILE *err_log;
  FILE *out_log;
  LogLevel level;
} Logger;

int logger_init(Logger *logger, const Options *opts);
void logger_close(Logger *Logger);

void log_message(Logger *logger, const LogLevel level, const char *message);

void log_error(Logger *logger, const char *message);
void log_info(Logger *logger, const char *message);
void log_warning(Logger *logger, const char *message);
void log_debug(Logger *logger, const char *message);

void log_info_formatted(Logger *logger, const char *format, ...);
void log_debug_formatted(Logger *logger, const char *format, ...);

#endif // LOGGER_H
