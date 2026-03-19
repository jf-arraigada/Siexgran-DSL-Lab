#include "siex-dsl/arena.h"
#include <siex-dsl/vector.h>
#include <stddef.h>
#include <string.h>


int vector_init(vector *v, size_t elem_size) {
  if (!v || elem_size == 0) return 1;

  v->data = NULL;
  v->count = 0;
  v->capacity = 0;
  v->elem_size = elem_size;

  return 0;
}

int vector_push(arena *a, vector *v, void *elem) {
  if (!a || !v || !elem) return 1;

  // grow
  if (v->count == v->capacity) {
    size_t new_capacity = (v->capacity == 0) ? 4 : v->capacity * 2;

    void* new_data = arena_alloc(a, new_capacity * v->elem_size);
    if (!new_data) return 1;

    if (v->data) {
      memcpy(new_data, v->data, v->count * v->elem_size);
    }

    v->data = new_data;
    v->capacity = new_capacity;
  }

  void* dest = (char*)v->data + (v->count * v->elem_size);
  memcpy(dest, elem, v->elem_size);

  v->count++;
  return 0;
}

void* vector_get(vector* v, size_t index) {
  if (!v || index >= v->count) return NULL;
  
  return (char*)v->data + (index * v->elem_size);
}


