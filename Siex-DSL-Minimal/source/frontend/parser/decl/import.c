#include <siex-dsl/frontend/parser.h>
#include <siex-dsl/frontend/parser_internal.h>

int parse_import_decl(parser *p) {
  token t = parser_peek(p);

  if (t.type != TOK_KEYWORD_IMPORT) {
    parser_error(t, "Expected 'import' keyword");
    return 1;
  }

  parser_next(p);
  t = parser_peek(p);

  if (t.type != TOK_STRING) {
    parser_error(t, "Expected string after 'import'");
    return 1;
  }

  char* path = arena_strdup(p->memory, t.token_start + 1, t.length - 2);

  parser_next(p);
  t = parser_peek(p);

  char* alias = NULL;

  if (t.type == TOK_KEYWORD_AS) {
    parser_next(p);

    t = parser_peek(p);
    if (t.type != TOK_IDENTIFIER) {
      parser_error(t, "Expected alias name after 'as'");
      return 1;
    }

    alias = arena_strdup(p->memory, t.token_start, t.length);
    parser_next(p);
    t = parser_peek(p);
  }

  if (t.type != TOK_SEMICOLON) {
    parser_error(t, "Expected ';' after import declaration");
    return 1;
  }

  parser_next(p);

  siex_import* import = arena_new_zero(p->memory, siex_import);
  import->path = path;
  import->alias = alias;

  vector_push(p->memory, &p->program.imports, &import);

  return 0;
}
