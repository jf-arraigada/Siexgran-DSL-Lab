#ifndef SIEX_DSL_HASHMAP_H
#define SIEX_DSL_HASHMAP_H

#include <stddef.h>
#include <stdint.h>
#include <siex-dsl/core/arena.h>
#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

typedef struct {
  const char* key;
  void* value;
  uint32_t hash;
  uint32_t distance;
} hashmap_entry;

typedef struct {
  hashmap_entry* entries;
  arena* memory;
  size_t capacity;
  size_t size;
} hashmap;

// API

int hashmap_init(hashmap* hm, arena* memory, size_t capacity);

int hashmap_put(hashmap* hm, const char* key, void* value);

void* hashmap_get(hashmap* hm, const char* key);

int hashmap_exists(hashmap* hm, const char* key);

int hashmap_put_hashed(hashmap* hm, const char* key, uint32_t hash, void* value);

void* hashmap_get_hashed(hashmap* hm, const char* key, uint32_t hash);


// hash

uint32_t hashmap_hash_str(const char* str);

#ifdef __cplusplus
}
#endif // !__cplusplus

#endif // SIEX_DSL_HASHMAP_H
