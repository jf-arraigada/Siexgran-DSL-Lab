#include <stdio.h>
#include <string.h>
#include <siex-dsl/core/namespace.h>
#include <siex-dsl/core/hashmap.h>

static const char* namespace_file_basename(arena* mem, const char* path) {
    const char* last_slash = strrchr(path, '/');
#ifdef _WIN32
    const char* last_backslash = strrchr(path, '\\');
    if (!last_slash || (last_backslash && last_backslash > last_slash))
        last_slash = last_backslash;
#endif

    const char* filename = last_slash ? last_slash + 1 : path;

    size_t len = strlen(filename);

    // remover ".siex"
    if (len > 5 && strcmp(filename + len - 5, ".siex") == 0) {
        len -= 5;
    }

    return arena_strdup(mem, filename, len);
}

const char* namespace_build(arena* memory, const char* file, const char* name) {
    const char* base = namespace_file_basename(memory, file);

    size_t len_file = strlen(base);
    size_t len_name = strlen(name);

    size_t total = len_file + 2 + len_name + 1;

    char* out = arena_alloc(memory, total);
    if (!out) return NULL;

    memcpy(out, base, len_file);
    memcpy(out + len_file, "::", 2);
    memcpy(out + len_file + 2, name, len_name);

    out[total - 1] = '\0';

    return out;
}

siex_symbol_id namespace_symbol(arena* mem, const char* file, const char* name) {
    const char* full = namespace_build(mem, file, name);

    siex_symbol_id id;
    id.full = full;
    id.hash = hashmap_hash_str(full);

    return id;
}

