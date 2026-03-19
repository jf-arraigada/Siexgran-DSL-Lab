#ifndef SIEX_DSL_COMMON_H
#define SIEX_DSL_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdlib.h>

typedef struct source_location {
  size_t line;
  size_t column;
} source_location;


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !SIEX_DSL_COMMON_H
