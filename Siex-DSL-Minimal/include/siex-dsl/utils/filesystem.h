#ifndef SIEX_DSL_FILESYSTEM_H
#define SIEX_DSL_FILESYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

#include <siex-dsl/core/arena.h>

// exists
int filesystem_file_exists(const char* path);
int filesystem_dir_exists(const char* path);

// path utils
char* filesystem_expand_env(arena* mem, const char* path);
char* filesystem_resolve_absolute(arena* mem, const char* path);
char* filesystem_dirname(arena* mem, const char* path);
char* filesystem_resolve_full(arena* mem, const char* base_dir, const char* path);

#ifdef __cplusplus
}
#endif // !__cplusplus

#endif // !SIEX_DSL_FILESYSTEM_H
