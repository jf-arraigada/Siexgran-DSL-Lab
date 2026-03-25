#include <siex-dsl/frontend/parser.h>
#include <siex-dsl/frontend/parser_internal.h>

static int parse_impl_decl(parser* p, siex_backend* backend);

int parse_backend_decl(parser *p) {
  token t = parser_peek(p);
  if (t.type != TOK_KEYWORD_BACKEND) {
    parser_error(t, "Expected 'backend' keyword");
    return 1;
  }

  parser_next(p);
  t = parser_peek(p);

  if (t.type != TOK_IDENTIFIER) {
    parser_error(t, "Expected identifier after 'backend'");
    return 1;
  }

  char* name = arena_strdup(p->memory, t.token_start, t.length);

  siex_backend* backend = arena_new_zero(p->memory, siex_backend);
  backend->name = name;
  vector_init_t(&backend->implementations, siex_impl*);

  parser_next(p); // consume name
  t = parser_peek(p);

  if (t.type == TOK_SEMICOLON) {
    parser_next(p);
  }
  else if (t.type == TOK_LBRACE) {
    parser_next(p); // consume {

    while ((t = parser_peek(p)).type != TOK_RBRACE && t.type != TOK_EOF) {
      if (t.type == TOK_KEYWORD_IMPL) {
        if (parse_impl_decl(p, backend) != 0)
        return 1;
      } 
      else {
        parser_error(t, "Unexpected token in backend");
        return 1;
      } 
    }
    if (t.type != TOK_RBRACE) {
      parser_error(t, "Expected '}' after backend block");
      return 1;
    }

    parser_next(p); // }
    t = parser_peek(p);

    if (t.type != TOK_SEMICOLON) {
      parser_error(t, "Expected ';' after backend declaration");
      return 1;
    }

    parser_next(p); // ';'
  }
  else {
    parser_error(t, "Expected ';' or '{' after backend name");
    return 1;
  }
  

  vector_push(p->memory, &p->program.backends, &backend);
  return 0;
}

// ------------------- LOCAL FUNCTION --------------------

static int parse_impl_decl(parser* p, siex_backend* backend) {
  token t = parser_peek(p);

  if (t.type != TOK_KEYWORD_IMPL) {
    parser_error(t, "Expected 'impl' keyword");
    return 1;
  }

  parser_next(p); // impl
  t = parser_peek(p);

  if (t.type != TOK_IDENTIFIER) {
    parser_error(t, "Expected identifier after 'impl'");
    return 1;
  }

  char* name = arena_strdup(p->memory, t.token_start, t.length);

  siex_impl* impl = arena_new_zero(p->memory, siex_impl);
  impl->name = name;

  parser_next(p);
  t = parser_peek(p);

  if (t.type != TOK_SEMICOLON) {
    parser_error(t, "Expected ';' after impl");
    return 1;
  }

  parser_next(p);

  vector_push(p->memory, &backend->implementations, &impl);

  return 0;
}


