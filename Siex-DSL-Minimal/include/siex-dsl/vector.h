#ifndef SIEX_DSL_VECTOR_H
#define SIEX_DSL_VECTOR_H

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

#include <siex-dsl/arena.h>

#define vector_push_val(a, v, value) \
  do { \
    __typeof__(value) tmp = (value); \
    vector_push((a), (v), &tmp); \
  } while(0)

#define vector_push_ptr(a, v, ptr) \
  vector_push((a), (v), &(ptr))

#define vector_init_t(v, type) \
  vector_init((v), sizeof(type))

#define vector_foreach(v, type, it) \
  for (size_t _i = 0; \
       _i < (v)->count && ((it) = (type*)((char*)(v)->data + _i * (v)->elem_size)); \
       _i++)

#define vector_get_t(v, type, index) \
  ((type*)vector_get((v), (index)))

#define vector_get_val(v, type, index) \
  (*(type*)vector_get((v), (index)))


typedef struct {
    void* data;
    size_t count;
    size_t capacity;
    size_t elem_size;
} vector;

int vector_init(vector* v, size_t elem_size);

int vector_push(arena* a, vector* v, void* elem);

void* vector_get(vector* v, size_t index);

#ifdef __cplusplus
}
#endif // !__cplusplus

#endif // !SIEX_DSL_VECTOR_H
