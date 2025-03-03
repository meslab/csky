#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

typedef enum { LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR } LogLevel;

FILE *err_log = NULL;
FILE *out_log = NULL;
LogLevel current_log_level = LOG_INFO;

const char *log_level_str(LogLevel level) {
  switch (level) {
  case LOG_DEBUG:
    return "DEBUG";
  case LOG_INFO:
    return "INFO";
  case LOG_WARNING:
    return "WARNING";

  default:
    return "ERROR";
  }
}

void log_message(LogLevel level, const char *message);
void log_error(const char *message);
void log_info(const char *message);
void log_warning(const char *message);
void log_debug(const char *message);

void close_log_files();

#endif // LOGGER_H
