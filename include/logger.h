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

int init_logger(Logger *logger, const Options *opts);
void close_logger(Logger *Logger);

void log_message(Logger *logger, LogLevel level, const char *message);

void log_error(Logger *logger, const char *message);
void log_info(Logger *logger, const char *message);
void log_warning(Logger *logger, const char *message);
void log_debug(Logger *logger, const char *message);

void log_info_formatted(Logger *logger, const char *format, ...);

#endif // LOGGER_H
