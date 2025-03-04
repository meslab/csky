#include "../include/squitterrator.h"

int main(int argc, char *argv[]) {
  Options opts;
  parse_arguments(argc, argv, &opts);

  Logger logger;
  init_logger(&logger, &opts);

  ring_buffer_t ring_buffer;
  init_ring_buffer(&ring_buffer);

  pthread_t client_thread, processor_thread;

  ClientArgs client_args = {&ring_buffer, &opts, &logger};
  ProcessorArgs processor_args = {&ring_buffer, &opts, &logger};

  pthread_create(&client_thread, NULL, tcp_client_thread, &client_args);
  pthread_create(&processor_thread, NULL, data_processor_thread,
                 &processor_args);

  pthread_join(client_thread, NULL);
  pthread_join(processor_thread, NULL);

  close_logger();

  return 0;
}
