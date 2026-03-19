#include <siex-dsl/arena.h>

#include <stddef.h>
#include <stdint.h>
#include <string.h>


#define DEFAULT_ALIGN _Alignof(max_align_t)

  /* let ptr = 1003, in binary ptr = 1111101011
   * let align = 8, in binary align = 1000
   * so,(align - 1) = 7, in binary (align - 1) = 0111
   * We know from mathematics a module of a division is the rest of the division:
   * a = b * q + r, a = number, b = divisor, q = integer quotient, r = rest
   * We can resolve a simple division like:
   * a = 1003, b = 8
   * a/b = 1003/8 = 125.375
   * if we catch the integer part of the result, we have q = 125
   * and now multiplies q with b, we'd have
   * b*q = 8*125 = 1000
   * and knowing that, we can
   * a - b*q = 1003 - 8*125 = 1003 - 1000 = 3
   * and 3 it's finally the rest of the division by a/b
   * But, if b is 2^n, where n is an integer, we can resume that like
   * a%b = a & (b - 1)
   * a=1111101011 (1003), b = 1000 (8), so (b - 1) = 0111 (7)
   * a & (b - 1) = 
   *       1111101011   (1003)
   *    &  0000000111   (7)
   *    -------------
   *      0000000011   = 3
   */
static uintptr_t align_forward(uintptr_t ptr, size_t align) {
  uintptr_t module_of = ptr & (align - 1);
  if (module_of != 0)
    ptr += (align - module_of);
  return ptr;
}

void* arena_alloc_aligned(arena* a, size_t size, size_t align) {
  uintptr_t current = (uintptr_t)(a->memory + a->offset);
  uintptr_t aligned_address = align_forward(current, align);

  size_t new_offset = (aligned_address - (uintptr_t)(a->memory)) + size;

  if (new_offset > a->size) {
    return NULL;
  }

  a->offset = new_offset;
  return (void*)aligned_address; // interpret this address like a pointer to memory
}

void* arena_calloc_aligned(arena* a, size_t size, size_t align) {
  void* ptr = arena_alloc_aligned(a, size, align);
  if (ptr) memset(ptr, 0, size);
  return ptr;
}

void* arena_alloc(arena* a, size_t size) {
  return arena_alloc_aligned(a, size, DEFAULT_ALIGN);
}

void* arena_calloc(arena* a, size_t size) {
  return arena_calloc_aligned(a, size, DEFAULT_ALIGN);
}

int arena_init(arena *a, size_t size) {
  if (!a || size == 0) return 1;

  a->memory = malloc(size);
  if (!a->memory) return 1;

  a->size = size;
  a->offset = 0;

  return 0;
}

void arena_free(arena *a) {
  if (!a) return;

  free(a->memory);
  a->memory = NULL;
  a->size = 0;
  a->offset = 0;
}

void arena_reset(arena *a) {
  if (!a) return;

  a->offset = 0;
}

char* arena_strdup(arena* a, const char* start, size_t len) {
  char* string = arena_alloc(a, len + 1);
  if(!string) return NULL;

  memcpy(string, start, len);
  string[len] = '\0';

  return string;
}
