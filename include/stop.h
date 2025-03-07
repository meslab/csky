#ifndef STOP_H
#define STOP_H

#include <signal.h>

extern volatile sig_atomic_t stop;
void handle_sigint(int sig __attribute__((unused)));

#endif // STOP_H