#include "../include/squitterrator.h"

int main(int argc, char *argv[]) {
  Options opts;
  parse_arguments(argc, argv, &opts);

  MemoryArena arena;
  arena_init(&arena, 1024 * 1024);

  Logger *logger = (Logger *)arena_alloc(&arena, sizeof(Logger));
  init_logger(logger, &opts);

  ring_buffer_t *ring_buffer = arena_alloc(&arena, sizeof(ring_buffer));
  init_ring_buffer(ring_buffer);

  pthread_t client_thread, processor_thread;

  ClientArgs *client_args =
      (ClientArgs *)arena_alloc(&arena, sizeof(ClientArgs));
  client_args->rb = ring_buffer;
  client_args->opts = &opts;
  client_args->logger = logger;

  ProcessorArgs *processor_args =
      (ProcessorArgs *)arena_alloc(&arena, sizeof(ProcessorArgs));
  processor_args->rb = ring_buffer;
  processor_args->opts = &opts;
  processor_args->logger = logger;

  pthread_create(&client_thread, NULL, tcp_client_thread, &client_args);
  pthread_create(&processor_thread, NULL, data_processor_thread,
                 &processor_args);

  pthread_join(client_thread, NULL);
  pthread_join(processor_thread, NULL);

  close_logger(logger);

  arena_free(&arena);

  return 0;
}
