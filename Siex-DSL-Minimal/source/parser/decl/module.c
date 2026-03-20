#include <siex-dsl/parser.h>
#include <siex-dsl/parser_internal.h>

static int parse_need_decl(parser* p, siex_module* module);
static int parse_module_sources_block(parser* p, siex_module* module);
static int parse_module_include_block(parser* p, siex_module* module);


int parse_module_decl(parser *p) {
  token t = parser_peek(p);

  if (t.type != TOK_KEYWORD_MODULE) {
    parser_error(t, "Expected 'module' keyword");
    return 1;
  }

  parser_next(p); // module

  t = parser_peek(p);
  if (t.type != TOK_IDENTIFIER) {
    parser_error(t, "Expected module name");
    return 1;
  }

  char* name = arena_strdup(&p->mem, t.token_start, t.length);
  parser_next(p);

  t = parser_peek(p);
  if (t.type != TOK_LBRACE) {
    parser_error(t, "Expected '{'");
    return 1;
  }

  parser_next(p); // consumir '{'

  // ahora sí crear módulo
  siex_module* module = arena_new_zero(&p->mem, siex_module);

  module->name = name;

  vector_init_t(&module->needs, char*);
  vector_init_t(&module->sources, char*);
  vector_init_t(&module->includes, char*);

  while ((t = parser_peek(p)).type != TOK_RBRACE && t.type != TOK_EOF) {

  switch (t.type) {
    case TOK_KEYWORD_NEED:
      if (parse_need_decl(p, module) != 0)
        return 1;
      break;

    case TOK_KEYWORD_SOURCES:
      if (parse_module_sources_block(p, module) != 0)
        return 1;
      break;
    case TOK_KEYWORD_INCLUDES:
      if (parse_module_include_block(p, module) != 0)
        return 1;
      break;

    default:
      parser_error(t, "Unexpected token in module");
      return 1;
    }
  }

  if (t.type != TOK_RBRACE) {
    parser_error(t, "Expected '}' after module block");
    return 1;
  }

  parser_next(p); // consumir '}'
  t = parser_peek(p);

  if (t.type != TOK_SEMICOLON) {
    parser_error(t, "Expected ';' after module declaration");
    return 1;
  }

  parser_next(p); // consumir ';'

  // guardar módulo
  vector_push(&p->mem, &p->program.modules, &module);

  return 0;
}

// -------------- LOCAL FUNCTIONS ---------------

static int parse_need_decl(parser* p, siex_module* module) {
  token t = parser_peek(p);

  if (t.type != TOK_KEYWORD_NEED) {
    parser_error(t, "Expected 'need' keyword");
    return 1;
  }

  parser_next(p); // consumir 'need'
  t = parser_peek(p);

  if (t.type != TOK_IDENTIFIER) {
    parser_error(t, "Expected identifier after 'need'");
    return 1;
  }

  char* need_name = arena_strdup(&p->mem, t.token_start, t.length);

  parser_next(p);
  t = parser_peek(p);

  if (t.type != TOK_SEMICOLON) {
    parser_error(t, "Expected ';' after need");
    return 1;
  }

  parser_next(p); // consumir ';'

  // acá entra vector
  vector_push(&p->mem, &module->needs, &need_name);

  return 0;
}

static int parse_module_sources_block(parser* p, siex_module* module) {
  if ((parse_block(p, module, MODULE_BLOCK, "sources")) != 0)
    return 1;
  return 0;
}

static int parse_module_include_block(parser* p, siex_module* module) {
  if ((parse_block(p, module, MODULE_BLOCK, "includes")) != 0)
    return 1;
  return 0;
}

