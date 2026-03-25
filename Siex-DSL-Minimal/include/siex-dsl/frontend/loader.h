#ifndef SIEX_DSL_LOADER_H
#define SIEX_DSL_LOADER_H

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

#include <siex-dsl/core/arena.h>
#include <siex-dsl/core/vector.h>
#include <siex-dsl/core/hashmap.h>
#include <siex-dsl/ir/ir.h>
#include <siex-dsl/frontend/lexer.h>
#include <siex-dsl/frontend/parser.h>

typedef struct {
    arena* memory;

    vector programs;     // siex_program*
    hashmap loaded;      // path → siex_program* (dedup)

} siex_loader;

// init
int siex_loader_init(siex_loader* loader, arena* mem);

// carga root + imports recursivos
int siex_loader_load(siex_loader* loader, const char* entry_path);

// acceso
vector* siex_loader_programs(siex_loader* loader);

#ifdef __cplusplus
}
#endif // !__cplusplus

#endif // !SIEX_DSL_LOADER_H
