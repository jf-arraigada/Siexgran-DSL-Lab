#include <siex-dsl/parser.h>
#include <siex-dsl/parser_internal.h>
#include <string.h>

int parse_root_decl(parser *p) {
  token t = parser_peek(p);
  
  if(t.type != TOK_KEYWORD_ROOT) {
    parser_error(t, "Expected 'root' keyword");
    return 1;
  }

  parser_next(p);
  t = parser_peek(p);

  if (t.type != TOK_IDENTIFIER) {
    parser_error(t, "Expected identifier after 'root'");
    return 1;
  }
  
  char* name = arena_strdup(&p->mem, t.token_start, t.length);
  if (!name) return 1;

  p->program.root_name = name;

  parser_next(p);
  t = parser_peek(p);

  if (t.type != TOK_EQUAL) {
    parser_error(t, "Expected '=' after root identifier");
    return 1;
  }

  parser_next(p);
  t = parser_peek(p);

  if (t.type != TOK_IDENTIFIER) {
    parser_error(t, "Expected root type after '='");
    return 1;
  }

  
  if (strncmp(t.token_start, "executable", t.length) == 0) {
    p->program.root_type = SIEX_ROOT_EXECUTABLE;
  } else if (strncmp(t.token_start, "library", t.length) == 0) {
    p->program.root_type = SIEX_ROOT_LIBRARY;
  } else if (strncmp(t.token_start, "plugin", t.length) == 0) {
    p->program.root_type = SIEX_ROOT_PLUGIN;
  } else {
    parser_error(t, "Unknown root type (expected executable/library/plugin)");
    return 1;
  }

  parser_next(p);
  t = parser_peek(p);

  if (t.type != TOK_SEMICOLON) {
    parser_error(t, "Expected ';' after root declaration");
    return 1;
  }

  parser_next(p);

  printf("parsed root: %s (%d)\n", name, p->program.root_type);
  return 0;
}

