#ifndef ARENA_H
#define ARENA_H

#include "../include/logger.h"
#include <stddef.h>
#include <stdint.h>

typedef struct MemoryArena {
  uint8_t *buffer;
  size_t size;
  size_t offset;
} MemoryArena;

void arena_init(MemoryArena *arena, size_t size);
void *arena_alloc(MemoryArena *arena, size_t size);
void arena_reset(MemoryArena *arena);
void arena_free(MemoryArena *arena);
void arena_info(Logger *logger, MemoryArena *arena);

#endif // ARENA_H
