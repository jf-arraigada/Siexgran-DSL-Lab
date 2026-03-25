#include <siex-dsl/frontend/parser.h>
#include <siex-dsl/frontend/parser_internal.h>
#include <string.h>

int init_parser(parser *p, lexer *lex) {
  if(!p || !lex) return 1;
  arena_init(p->memory, 1024 * 1024);
  memset(&p->program, 0, sizeof(siex_program));
  p->lex = lex;
  p->current = lexer_next_token(lex);

  vector_init_t(&p->program.modules, siex_module*);
  vector_init_t(&p->program.backends, siex_backend*);
  vector_init_t(&p->program.bindings, siex_binding*);
  vector_init_t(&p->program.imports, siex_import*);

  return 0;
}

int parse_program(parser* p) {
  while (parser_peek(p).type != TOK_EOF) {
    token t = parser_peek(p);

    switch (t.type) {
      case TOK_KEYWORD_ROOT:
        if (parse_root_decl(p) != 0) return 1;
        break;

      case TOK_KEYWORD_MODULE:
        if (parse_module_decl(p) != 0) return 1;
        break;

      case TOK_KEYWORD_BACKEND:
        if (parse_backend_decl(p) != 0) return 1;
        break;

      case TOK_KEYWORD_BIND:
        if (parse_bind_decl(p) != 0) return 1;
        break;

      case TOK_KEYWORD_TARGET:
        if (parse_target_decl(p) != 0) return 1;
        break;

      case TOK_KEYWORD_IMPORT:
        if (parse_import_decl(p) != 0) return 1;
        break;

      default:
        parser_error(t, "Unexpected token at top level");
        return 1;
    }
  }

  return 0;
}




