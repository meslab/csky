#include "../include/stop.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

volatile sig_atomic_t stop = 0;
void handle_sigint(int sig) {
  stop = 1;
  printf("\nReceived signal: %s\n", strsignal(sig));
  return;
}
