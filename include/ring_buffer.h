#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <pthread.h>

#define BUFFER_SIZE (1 << 5)         // Must be a power of 2
#define BUFFER_AND (BUFFER_SIZE - 1) // Modulo (%) optimisation
#define MAX_LINE_LENGTH 63           // Max length of a single line

typedef struct {
  char buffer[BUFFER_SIZE][MAX_LINE_LENGTH + 1]; // Extra byte for '\0'
  volatile int head;
  volatile int tail;
} ring_buffer_t;

void init_ring_buffer(ring_buffer_t *rb);
void ring_buffer_insert(ring_buffer_t *rb, const char *line);
int ring_buffer_get(ring_buffer_t *rb, char *line);

#endif // RING_BUFFER_H
