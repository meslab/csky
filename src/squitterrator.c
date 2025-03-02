#include "../include/arg_parser.h"

#include <arpa/inet.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 10     // Number of stored lines
#define MAX_LINE_LENGTH 64 // Max length of a single line

typedef struct {
  char buffer[BUFFER_SIZE]
             [MAX_LINE_LENGTH + 1]; // Stores lines (extra byte for '\0')
  int head;
  int tail;
  pthread_mutex_t mutex;
  pthread_cond_t not_empty;
  pthread_cond_t not_full;
} ring_buffer_t;

typedef struct {
  ring_buffer_t *rb;
  Options *opts;
} ThreadArgs;

// Initialize the ring buffer
void init_ring_buffer(ring_buffer_t *rb) {
  rb->head = 0;
  rb->tail = 0;
  pthread_mutex_init(&rb->mutex, NULL);
  pthread_cond_init(&rb->not_empty, NULL);
  pthread_cond_init(&rb->not_full, NULL);
}

// Insert a new line into the ring buffer
void ring_buffer_insert(ring_buffer_t *rb, const char *line) {
  pthread_mutex_lock(&rb->mutex);
  while ((rb->head + 1) % BUFFER_SIZE == rb->tail) {
    pthread_cond_wait(&rb->not_full, &rb->mutex); // Wait if buffer is full
  }
  strncpy(rb->buffer[rb->head], line, MAX_LINE_LENGTH);
  rb->buffer[rb->head][MAX_LINE_LENGTH] = '\0'; // Ensure null termination
  rb->head = (rb->head + 1) % BUFFER_SIZE;
  pthread_cond_signal(&rb->not_empty); // Signal that buffer is not empty
  pthread_mutex_unlock(&rb->mutex);
}

// Retrieve a line from the ring buffer
int ring_buffer_get(ring_buffer_t *rb, char *line) {
  pthread_mutex_lock(&rb->mutex);
  while (rb->head == rb->tail) {
    pthread_cond_wait(&rb->not_empty, &rb->mutex); // Wait if buffer is empty
  }
  strncpy(line, rb->buffer[rb->tail], MAX_LINE_LENGTH);
  line[MAX_LINE_LENGTH] = '\0';
  rb->tail = (rb->tail + 1) % BUFFER_SIZE;
  pthread_cond_signal(&rb->not_full); // Signal that buffer is not full
  pthread_mutex_unlock(&rb->mutex);
  return 0;
}

// Convert a hex string to an array of uint32_t values
void hex_to_uint32(const char *hex_str, uint32_t *arr, int *count) {
  *count = 0;
  for (size_t i = 0; i < strlen(hex_str) && i + 8 <= strlen(hex_str); i += 8) {
    char chunk[9] = {0};
    strncpy(chunk, hex_str + i, 8);
    arr[(*count)++] = strtoul(chunk, NULL, 16);
  }
}

// TCP client thread: Connects to server, reads data, and stores in ring buffer
void *tcp_client_thread(void *arg) {
  ThreadArgs *args = (ThreadArgs *)arg;

  Options *opts = args->opts;
  ring_buffer_t *rb = args->rb;

  int sockfd;
  struct sockaddr_in server_addr;

  // Create socket
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  // Set up server address
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(opts->tcp_port);
  inet_pton(AF_INET, opts->tcp_server, &server_addr.sin_addr);

  printf("Connecting to server %s:%d\n", opts->tcp_server, opts->tcp_port);

  // Connect to server
  if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
      -1) {
    perror("Connection failed");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  printf("Connected to server %s:%d\n", opts->tcp_server, opts->tcp_port);

  char buffer[128];               // Temporary buffer
  char line[MAX_LINE_LENGTH + 1]; // Stores extracted line
  int line_pos = 0;

  while (1) {
    ssize_t bytes_read = read(sockfd, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0)
      break; // Exit on error or disconnect

    buffer[bytes_read] = '\0';

    // Process the buffer character by character
    for (int i = 0; i < bytes_read; i++) {
      if (buffer[i] == '\n' || line_pos >= MAX_LINE_LENGTH) {
        line[line_pos] = '\0';
        if (line_pos > 0) {
          ring_buffer_insert(rb, line);
        }
        line_pos = 0;
      } else {
        line[line_pos++] = buffer[i];
      }
    }
  }

  printf("Server disconnected.\n");
  close(sockfd);
  return NULL;
}

// Data processing thread: Reads from ring buffer, processes hex data
void *data_processor_thread(void *arg) {
  ring_buffer_t *rb = (ring_buffer_t *)arg;
  uint32_t processed_data[8];
  int count;

  while (1) {
    char line[MAX_LINE_LENGTH + 1];
    if (ring_buffer_get(rb, line) == 0) {
      hex_to_uint32(line, processed_data, &count);
      printf("Processed data: ");
      for (int i = 0; i < count; i++) {
        printf("%u ", processed_data[i]);
      }
      printf("\n");
    }
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  Options opts;
  parse_arguments(argc, argv, &opts);

  ring_buffer_t ring_buffer;
  init_ring_buffer(&ring_buffer);

  pthread_t client_thread, processor_thread;

  ThreadArgs thread_args = {&ring_buffer, &opts};

  pthread_create(&client_thread, NULL, tcp_client_thread, &thread_args);
  pthread_create(&processor_thread, NULL, data_processor_thread, &ring_buffer);

  pthread_join(client_thread, NULL);
  pthread_join(processor_thread, NULL);

  return 0;
}
