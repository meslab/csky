#include "../include/client.h"

#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

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