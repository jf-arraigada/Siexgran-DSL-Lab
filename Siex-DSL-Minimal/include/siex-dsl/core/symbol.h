#ifndef SIEX_DSL_SYMBOL_H
#define SIEX_DSL_SYMBOL_H

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

#include <stdint.h>

typedef struct {
    const char* full;  // "file::name"
    uint32_t hash;
} siex_symbol_id;

#ifdef __cplusplus
}
#endif // !__cplusplus

#endif // !SIEX_DSL_SYMBOL_H
