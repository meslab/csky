#include "../include/squitterrator.h"

int main(int argc, char *argv[]) {
  Options opts;
  parse_arguments(argc, argv, &opts);

  init_logger(&opts);

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
