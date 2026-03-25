#include <siex-dsl/frontend/loader.h>
#include <siex-dsl/utils/filesystem.h>

static int loader_load_program(arena* memory, const char* path, siex_program* out) {
    // 1. resolver path absoluto
    const char* abs_path = filesystem_resolve_absolute(memory, path);

    // 2. crear file
    siex_file* file = arena_new_zero(memory, siex_file);
    file->path = abs_path;
    file->dir  = filesystem_dirname(memory, abs_path);

    
    // 3. lexer
    lexer lex;
    init_lexer(&lex, abs_path);

    // 4. parser
    parser p;
    p.memory = memory;
    init_parser(&p, &lex);

    // 5. parse
    if (parse_program(&p) != 0)
        return 1;

    // 6. copiar program
    *out = p.program;

    hashmap_init(&out->aliases, memory, 32);

    for (size_t i = 0; i < out->imports.count; i++) {
    siex_import* import = vector_get_t(&out->imports, siex_import, i);

    if (import->alias) {
      if (hashmap_exists(&out->aliases, import->alias)) {
        // error: alias duplicado
        return 1;
      }

      hashmap_put(&out->aliases, import->alias, import);
    }
}

    // CLAVE
    out->file = file;

    free_lexer(&lex);

    return 0;
}


static int loader_load_internal(siex_loader* loader, const char* path) {
    char* abs_path = filesystem_resolve_absolute(loader->memory, path);
    if (!abs_path)
      return 1;
    
    // ya cargado?
    if (hashmap_exists(&loader->loaded, abs_path))
        return 0;

    // crear program
    siex_program* program = arena_new_zero(loader->memory, siex_program);
    if (!program) return 1;

    if (loader_load_program(loader->memory, abs_path, program) != 0)
        return 1;

    // registrar
    hashmap_put(&loader->loaded, program->file->path, program);
    vector_push(loader->memory, &loader->programs, &program);

    // recorrer imports
    for (size_t i = 0; i < program->imports.count; i++) {
        siex_import* imp = vector_get_t(&program->imports, siex_import, i);

        char* full = filesystem_resolve_full(
            loader->memory,
            program->file->dir,
            imp->path
        );

        if (!full)
            return 1;

        if (loader_load_internal(loader, full) != 0)
            return 1;
    }

    return 0;
}

int siex_loader_load(siex_loader* loader, const char* entry_path) {
    char* abs = filesystem_resolve_absolute(loader->memory, entry_path);
    if (!abs) return 1;

    return loader_load_internal(loader, abs);
}

