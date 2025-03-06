#include "../include/ring_buffer.h"

#include <string.h>

/**
 * Initialize the ring buffer
 */
void init_ring_buffer(ring_buffer_t *rb) {
  rb->head = 0;
  rb->tail = 0;
}

/**
 * Insert a new line into the ring buffer
 */
void ring_buffer_insert(ring_buffer_t *rb, const char *line) {
  int head = rb->head;
  int next_head = (head + 1) & BUFFER_AND;
  while (next_head == rb->tail) {
    return;
  }

  strncpy(rb->buffer[rb->head], line, MAX_LINE_LENGTH);
  rb->buffer[rb->head][MAX_LINE_LENGTH] = '\0'; // Ensure null termination
  __sync_synchronize();                         // Memory barrier
  rb->head = next_head;
}

/**
 * Retrieve a line from the ring buffer
 */
int ring_buffer_get(ring_buffer_t *rb, char *line) {
  int tail = rb->tail;
  if (tail == rb->head) {
    return -1; // Buffer is empty
  }
  strncpy(line, rb->buffer[rb->tail], MAX_LINE_LENGTH);
  line[MAX_LINE_LENGTH] = '\0';
  __sync_synchronize(); // Memory barrier
  rb->tail = (rb->tail + 1) & BUFFER_AND;
  return 0;
}
