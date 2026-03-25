#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <siex-dsl/utils/filesystem.h>
#include <siex-dsl/core/arena.h>

int filesystem_file_exists(const char* path) {
  struct stat s;
  if (stat(path, &s) == 0) {
    return S_ISREG(s.st_mode);
  }
  return 0;
}

int filesystem_dir_exists(const char* path) {
  struct stat s;
  if (stat(path, &s) == 0) {
    return S_ISDIR(s.st_mode);
  }
  return 0;
}


char* filesystem_expand_env(arena* mem, const char* path) {
    if (path[0] != '$')
        return (char*)path;

    const char* slash = strchr(path, '/');
    size_t var_len = slash ? (size_t)(slash - path - 1) : strlen(path) - 1;

    char var[128];
    strncpy(var, path + 1, var_len);
    var[var_len] = '\0';

    const char* value = getenv(var);
    if (!value)
        return NULL;

    size_t len = strlen(value) + (slash ? strlen(slash) : 0) + 1;
    char* result = arena_alloc(mem, len);

    strcpy(result, value);
    if (slash)
        strcat(result, slash);

    return result;
}

char* filesystem_dirname(arena* mem, const char* path) {
    const char* slash = strrchr(path, '/');

    if (!slash)
        return arena_strdup(mem, ".", 1);

    size_t len = slash - path;

    char* out = arena_alloc(mem, len + 1);
    memcpy(out, path, len);
    out[len] = '\0';

    return out;
}


char* filesystem_resolve_absolute(arena* mem, const char* path) {
    char buffer[PATH_MAX];

    if (!realpath(path, buffer))
        return NULL;

    return arena_strdup(mem, buffer, strlen(buffer));
}

char* filesystem_resolve_full(arena* mem, const char* base_dir, const char* path) {
    // 1. expand env
    char* expanded = filesystem_expand_env(mem, path);
    if (!expanded) return NULL;

    // 2. absoluto
    if (expanded[0] == '/')
        return filesystem_resolve_absolute(mem, expanded);

    // 3. relativo → base_dir + path
    size_t len = strlen(base_dir) + 1 + strlen(expanded) + 1;

    char* combined = arena_alloc(mem, len);
    if (!combined) return NULL;

    strcpy(combined, base_dir);
    strcat(combined, "/");
    strcat(combined, expanded);

    return filesystem_resolve_absolute(mem, combined);
}
