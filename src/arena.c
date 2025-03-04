#include "../include/arena.h"
#include <stdio.h>
#include <stdlib.h>

void arena_init(MemoryArena *arena, size_t size) {
  arena->buffer = (uint8_t *)malloc(size);
  if (arena->buffer == NULL) {
    perror("Failed to allocate memory for arena");
    exit(EXIT_FAILURE);
  }
  arena->size = size;
  arena->offset = 0;
}

void *arena_alloc(MemoryArena *arena, size_t size) {
  if (arena->offset + size > arena->size) {
    fprintf(stderr, "Out of memory in arena\n");
    exit(EXIT_FAILURE);
  }
  void *ptr = &arena->buffer[arena->offset];
  arena->offset += size;
  return ptr;
}

void arena_reset(MemoryArena *arena) { arena->offset = 0; }

void arena_free(MemoryArena *arena) {
  free(arena->buffer);
  arena->buffer = NULL;
  arena->size = 0;
  arena->offset = 0;
}

void arena_info(Logger *logger, MemoryArena *arena) {
  log_info_formatted(logger, "Arena size: %ld, offset: %ld\n", arena->size,
                     arena->offset);
}