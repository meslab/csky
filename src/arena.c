#include "../include/arena.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// @brief Initialize a memory arena
/// @param arena The memory arena to initialize
/// @param size  The size of the memory arena in bytes
void arena_init(MemoryArena *arena, size_t size) {
  arena->buffer = (uint8_t *)malloc(size);
  if (arena->buffer == NULL) {
    perror("Failed to allocate memory for arena");
    fprintf(stderr, "errno: %d, strerror: %s\n", errno, strerror(errno));
    exit(EXIT_FAILURE);
  }
  arena->size = size;
  arena->offset = 0;
}

/// @brief Allocate memory from the arena
/// @param arena The memory arena to allocate from
/// @param size  The size of the memory to allocate
/// @return A pointer to the allocated memory
void *arena_alloc(MemoryArena *arena, size_t size) {
  if (arena->offset + size > arena->size) {
    fprintf(stderr, "Out of memory in arena\n");
    exit(EXIT_FAILURE);
  }
  void *ptr = &arena->buffer[arena->offset];
  arena->offset += size;
  return ptr;
}

/// @brief  Reset the memory arena
/// @param arena The memory arena to reset to the beginning of the buffer
void arena_reset(MemoryArena *arena) { arena->offset = 0; }

/// @brief Free the memory allocated for the arena
/// @param arena The memory arena to free
void arena_free(MemoryArena *arena) {
  free(arena->buffer);
  arena->buffer = NULL;
  arena->size = 0;
  arena->offset = 0;
}

/// @brief Print information about the memory arena
/// @param logger The logger to use for printing
/// @param arena  The memory arena to print information about
void arena_info(Logger *logger, MemoryArena *arena) {
  log_info_formatted(logger, "Arena size: %ld, offset: %ld\n", arena->size,
                     arena->offset);
}