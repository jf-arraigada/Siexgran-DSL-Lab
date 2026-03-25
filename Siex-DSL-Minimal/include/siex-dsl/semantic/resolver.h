#ifndef SIEX_DSL_RESOLVER_H
#define SIEX_DSL_RESOLVER_H

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

#include <siex-dsl/core/arena.h>
#include <siex-dsl/core/hashmap.h>
#include <siex-dsl/ir/ir.h>
#include <siex-dsl/ir/resolved_ir.h>

typedef struct {
    arena* memory;

    vector* programs; // siex_program*

    hashmap module_table;   // symbol → siex_resolved_module*
    hashmap backend_table;  // symbol → siex_resolved_backend*
    hashmap target_table;  // symbol → siex_resolved_target*

} siex_resolver;

int siex_resolver_init(siex_resolver* resolver, arena* memory, vector* programs);

int siex_resolver_resolve(siex_resolver* resolver, siex_resolved_program* out);

#ifdef __cplusplus
}
#endif // !__cplusplus

#endif // SIEX_DSL_RESOLVER_H
