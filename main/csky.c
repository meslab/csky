#include "../../crux/include/linear_arena.h"
#include "../../crux/include/logger.h"
#include "../../crux/include/utils.h"
#include "../include/arg_parser.h"
#include "../include/client.h"
#include "../include/processor.h"
#include "../include/ring_buffer.h"

int main(int argc, char *argv[]) {
	Options opts;
	arguments_parse(argc, argv, &opts);

	LinearMemoryArena linear_arena;
	linear_arena_init(&linear_arena, 1024 * 1024);

	Logger *logger = (Logger *)linear_arena_alloc(&linear_arena, sizeof(Logger));
	logger_init(logger, NULL, NULL, opts.log_level);

	ringBuffer *ring_buffer = linear_arena_alloc(&linear_arena, sizeof(ring_buffer));
	ring_buffer_init(ring_buffer);

	pthread_t client_thread, processor_thread;

	TcpClientArgs *tcp_client_args =
		(TcpClientArgs *)linear_arena_alloc(&linear_arena, sizeof(TcpClientArgs));

	if (tcp_client_thread_init(tcp_client_args, ring_buffer, &opts, logger)) {
		error_log(logger, "TcpClientArgs init failed.");
		return -1;
	}
	pthread_create(&client_thread, NULL, tcp_client_thread, tcp_client_args);

	ProcessorArgs *processor_args =
		(ProcessorArgs *)linear_arena_alloc(&linear_arena, sizeof(ProcessorArgs));

	if (data_processor_thread_args_init(processor_args, ring_buffer, &opts,
				logger)) {
		error_log(logger, "ProcessorArgs init failed.");
		return -1;
	}

	pthread_create(&processor_thread, NULL, data_processor_thread,
			processor_args);

	linear_arena_status_update(&linear_arena);
	debug_log(logger, linear_arena.status);

	pthread_join(client_thread, NULL);
	pthread_join(processor_thread, NULL);

	logger_close(logger);

	linear_arena_free(&linear_arena);

	return 0;
}
