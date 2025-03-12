#include "../include/client.h"
#include "../include/logger.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief Initialize the TCP client arguments
 *
 * @param tcp_client_args TCP client arguments
 * @param ring_buffer Ring buffer
 * @param opts Options
 * @param logger Logger
 * @return int8_t 0 on success, -1 on failure
 */
inline int8_t tcp_client_thread_init(TcpClientArgs *tcp_client_args,
                                     ringBuffer *ring_buffer, Options *opts,
                                     Logger *logger) {
  if (!tcp_client_args) {
    return -1;
  }
  tcp_client_args->rb = ring_buffer;
  tcp_client_args->opts = opts;
  tcp_client_args->logger = logger;

  return 0;
}

/**
 * @brief TCP client thread
 *
 * @param arg Arguments
 * @return void*
 */
void *tcp_client_thread(void *arg) {
  TcpClientArgs *args = (TcpClientArgs *)arg;

  Options *opts = args->opts;
  ringBuffer *rb = args->rb;
  Logger *logger = args->logger;

  int sockfd;
  struct sockaddr_in server_addr;

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    error_log(logger, "Socket creation failed");
    exit(EXIT_FAILURE);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(opts->tcp_port);
  inet_pton(AF_INET, opts->tcp_server, &server_addr.sin_addr);

  info_log_formatted(logger, "Connecting to server %s:%d\n", opts->tcp_server,
                     opts->tcp_port);

  if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
      -1) {
    error_log(logger, "Connection failed");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  info_log_formatted(logger, "Connected to server %s:%d\n", opts->tcp_server,
                     opts->tcp_port);

  lines_read(sockfd, rb);

  info_log(logger, "Server disconnected.");
  close(sockfd);
  return NULL;
}

/**
 * @brief Read lines from the socket and insert them into the ring buffer
 *
 * @param sockfd Socket file descriptor
 * @param rb Ring buffer
 */
void lines_read(int sockfd, ringBuffer *rb) {
  char buffer[128 * 1024];        // Temporary buffer
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
}