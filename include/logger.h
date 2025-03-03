#ifndef LOGGER_H
#define LOGGER_H

#include "../include/arg_parser.h"
#include <stdio.h>
#include <string.h>

typedef enum { LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR } LogLevel;

static FILE *err_log = NULL;
static FILE *out_log = NULL;
static LogLevel current_log_level = LOG_ERROR;

const char *log_level_str(LogLevel level) {
  switch (level) {
  case LOG_DEBUG:
    return "DEBUG";
  case LOG_INFO:
    return "INFO";
  case LOG_WARNING:
    return "WARNING";
  case LOG_ERROR:
    return "ERROR";
  default:
    return "UNKNOWN";
  }
}

LogLevel parse_log_level(const char *level_str) {
  if (strcmp(level_str, "DEBUG") == 0)
    return LOG_DEBUG;
  if (strcmp(level_str, "INFO") == 0)
    return LOG_INFO;
  if (strcmp(level_str, "WARNING") == 0)
    return LOG_WARNING;
  return LOG_ERROR;
}

int init_logger(const Options *opts);
void log_message(LogLevel level, const char *message);
void log_error(const char *message);
void log_info(const char *message);
void log_warning(const char *message);
void log_debug(const char *message);

void close_log_files();

#endif // LOGGER_H
