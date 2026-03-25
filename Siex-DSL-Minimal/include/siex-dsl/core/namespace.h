#ifndef SIEX_DSL_NAMESPACE_H
#define SIEX_DSL_NAMESPACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <siex-dsl/core/arena.h>
#include <siex-dsl/core/symbol.h>

typedef struct {
    const char* file;   // path absoluto o ID
    const char* name;   // symbol (module/backend/etc)
} siex_namespace;

const char* namespace_build(arena* memory, const char* file, const char* name);

siex_symbol_id namespace_symbol(arena* mem, const char* file, const char* name);

#ifdef __cplusplus
}
#endif

#endif
