#ifndef SIEX_DSL_ARENA_H
#define SIEX_DSL_ARENA_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
      
#include <stdlib.h>

#define arena_new(a, type) \
  (type*)arena_alloc_aligned(a, sizeof(type), _Alignof(type))
#define arena_new_zero(a, type) \
  (type*)arena_calloc_aligned(a, sizeof(type), _Alignof(type))  


typedef struct {
  char* memory;
  size_t size;
  size_t offset;
} arena;

int arena_init(arena* a, size_t size);

void* arena_alloc(arena* a, size_t n);

void* arena_alloc_aligned(arena* a, size_t size, size_t align); 

void* arena_calloc(arena* a, size_t size);

void* arena_calloc_aligned(arena* a, size_t size, size_t align);

void arena_free(arena* a);

void arena_reset(arena* a);


char* arena_strdup(arena* a, const char* start, size_t len);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !SIEX_DSL_ARENA_H

