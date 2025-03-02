#include "../include/arg_parser.h"
#include "../include/client.h"
#include "../include/ring_buffer.h"
#include "../include/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
