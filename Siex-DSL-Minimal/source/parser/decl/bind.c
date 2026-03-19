#include <siex-dsl/parser.h>
#include <siex-dsl/parser_internal.h>

int parse_bind_decl(parser *p) {
  token t = parser_peek(p);

  if (t.type != TOK_KEYWORD_BIND) {
    parser_error(t, "Expected 'bind' keyword");
    return 1;
  }

  parser_next(p); // 'bind'
  t = parser_peek(p);

  if (t.type != TOK_IDENTIFIER) {
    parser_error(t, "Expected module name in bind");
    return 1;
  }
  
  char* module = arena_strdup(&p->mem, t.token_start, t.length);
  
  parser_next(p); // 'module_name'
  t = parser_peek(p);

  if (t.type != TOK_DOT) {
    parser_error(t, "Expected '.' in bind target");
    return 1;
  }

  parser_next(p); // '.'
  t = parser_peek(p);

  if (t.type != TOK_IDENTIFIER) {
    parser_error(t, "Expected service name in bind target");
    return 1;
  }

  char* service = arena_strdup(&p->mem, t.token_start, t.length);

  parser_next(p); // 'service_name'
  t = parser_peek(p);

  if (t.type != TOK_EQUAL) {
    parser_error(t, "Expected '=' in bind");
    return 1;
  }

  // bind value --------------------------

  parser_next(p); // '='
  t = parser_peek(p);

  if (t.type != TOK_IDENTIFIER) {
    parser_error(t, "Expected backend name in bind value");
    return 1;
  }

  char* backend = arena_strdup(&p->mem, t.token_start, t.length);
  char* impl = NULL;

  parser_next(p); // 'backend_name'
  t = parser_peek(p);

  if (t.type == TOK_DOT) {
    parser_next(p); // '.'
    t = parser_peek(p);

    if (t.type != TOK_IDENTIFIER) {
      parser_error(t, "Expected implementation name after '.'");
      return 1;
    }

    impl = arena_strdup(&p->mem, t.token_start, t.length);
    parser_next(p);
  }

  t = parser_peek(p);

  if (t.type != TOK_SEMICOLON) {
    parser_error(t, "Expected ';' after bind declaration");
    return 1;
  }

  parser_next(p);

  siex_binding* binding = arena_new_zero(&p->mem, siex_binding);

  binding->module = module;
  binding->service = service;
  binding->backend = backend;
  binding->impl = impl;

  vector_push(&p->mem, &p->program.bindings, &binding);

  return 0;
}

