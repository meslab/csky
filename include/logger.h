#ifndef LOGGER_H
#define LOGGER_H

#define LOG_MESSAGE_MAX_LENGTH 80

#include "../include/arg_parser.h"

typedef enum { LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR } LogLevel;

int init_logger(const Options *opts);
void close_logger();

void log_message(LogLevel level, const char *message);

void log_error(const char *message);
void log_info(const char *message);
void log_warning(const char *message);
void log_debug(const char *message);

void log_info_formatted(const char *format, ...);

#endif // LOGGER_H
