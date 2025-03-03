#include "../include/logger.h"

#include <time.h>

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