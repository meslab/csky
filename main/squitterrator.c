#include "../include/squitterrator.h"

int main(int argc, char *argv[]) {
  Options opts;
  arguments_parse(argc, argv, &opts);

  MemoryArena arena;
  arena_init(&arena, 1024 * 1024);

  Logger *logger = (Logger *)arena_alloc(&arena, sizeof(Logger));
  logger_init(logger, &opts);

  ring_buffer_t *ring_buffer = arena_alloc(&arena, sizeof(ring_buffer));
  init_ring_buffer(ring_buffer);

  pthread_t client_thread, processor_thread;

  TcpClientArgs *tcp_client_args =
      (TcpClientArgs *)arena_alloc(&arena, sizeof(TcpClientArgs));

  if (tcp_client_thread_init(tcp_client_args, ring_buffer, &opts, logger)) {
    error_log(logger, "TcpClientArgs init failed.");
    return -1;
  }
  pthread_create(&client_thread, NULL, tcp_client_thread, tcp_client_args);

  ProcessorArgs *processor_args =
      (ProcessorArgs *)arena_alloc(&arena, sizeof(ProcessorArgs));

  if (init_data_processor_args(processor_args, ring_buffer, &opts, logger)) {
    error_log(logger, "ProcessorArgs init failed.");
    return -1;
  }

  pthread_create(&processor_thread, NULL, data_processor_thread,
                 processor_args);

  arena_info(logger, &arena);

  pthread_join(client_thread, NULL);
  pthread_join(processor_thread, NULL);

  logger_close(logger);

  arena_free(&arena);

  return 0;
}
