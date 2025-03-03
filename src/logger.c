#include "../include/logger.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

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
  default:
    return "ERROR";
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

void log_message(LogLevel level, const char *message) {
  if (level < current_log_level)
    return;

  FILE *dest = (level == LOG_ERROR) ? (err_log ? err_log : stderr)
                                    : (out_log ? out_log : stdout);

  // Get timestamp
  time_t now = time(NULL);
  struct tm *tm_info = localtime(&now);
  char time_buf[20]; // YYYY-MM-DD HH:MM:SS
  strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);

  fprintf(dest, "[%s] [%s] %s\n", time_buf, log_level_str(level), message);
  fflush(dest);
}

void log_error(const char *message) { log_message(LOG_ERROR, message); }
void log_info(const char *message) { log_message(LOG_INFO, message); }
void log_warning(const char *message) { log_message(LOG_WARNING, message); }
void log_debug(const char *message) { log_message(LOG_DEBUG, message); }

int init_logger(const Options *opts) {
  if (!opts)
    return -1; // Ensure opts is valid

  // Set the log level
  current_log_level =
      parse_log_level(opts->log_level ? opts->log_level : "ERROR");

  // Open log files if paths are provided
  if (opts->err_log) {
    err_log = fopen(opts->err_log, "a");
    if (!err_log) {
      perror("Failed to open error log file");
      return -1;
    }
  }

  if (opts->out_log) {
    out_log = fopen(opts->out_log, "a");
    if (!out_log) {
      perror("Failed to open output log file");
      if (err_log)
        fclose(err_log);
      return -1;
    }
  }

  return 0; // Success
}

void close_logger() {
  if (err_log)
    fclose(err_log);
  if (out_log)
    fclose(out_log);
  err_log = out_log = NULL;
}