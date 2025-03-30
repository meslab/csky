#include "ring_buffer.h"
#include <string.h>

/**
 * Initialize the ring buffer
 */
void ring_buffer_init(ringBuffer *rb) {
	rb->head = 0;
	rb->tail = 0;
	pthread_mutex_init(&rb->mutex, NULL);
	pthread_cond_init(&rb->not_empty, NULL);
	pthread_cond_init(&rb->not_full, NULL);
}

/**
 * Insert a new line into the ring buffer
 */
void ring_buffer_insert(ringBuffer *rb, const char *line) {
	pthread_mutex_lock(&rb->mutex);
	while (((rb->head + 1) & BUFFER_MASK) == rb->tail) {
		pthread_cond_wait(&rb->not_full,
				  &rb->mutex); // Wait if buffer is full
	}
	strncpy(rb->buffer[rb->head], line, MAX_LINE_LENGTH);
	rb->buffer[rb->head][MAX_LINE_LENGTH] = '\0'; // Ensure null termination
	rb->head = (rb->head + 1) & BUFFER_MASK;
	pthread_cond_signal(&rb->not_empty); // Signal that buffer is not empty
	pthread_mutex_unlock(&rb->mutex);
}

/**
 * Retrieve a line from the ring buffer
 */
int ring_buffer_get(ringBuffer *rb, char *line) {
	pthread_mutex_lock(&rb->mutex);
	while (rb->head == rb->tail) {
		pthread_cond_wait(&rb->not_empty,
				  &rb->mutex); // Wait if buffer is empty
	}
	strncpy(line, rb->buffer[rb->tail], MAX_LINE_LENGTH);
	line[MAX_LINE_LENGTH] = '\0';
	rb->tail = (rb->tail + 1) & BUFFER_MASK;
	pthread_cond_signal(&rb->not_full); // Signal that buffer is not full
	pthread_mutex_unlock(&rb->mutex);
	return 0;
}
