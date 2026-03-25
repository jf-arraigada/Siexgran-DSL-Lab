#include <siex-dsl/frontend/parser.h>
#include <siex-dsl/frontend/parser_internal.h>
#include <string.h>

static int parse_target_sources_block(parser* p, siex_target* target);
static int parse_target_includes_block(parser* p, siex_target* target);


int parse_target_decl(parser* p) {
  token t = parser_peek(p);

  if (t.type != TOK_KEYWORD_TARGET) {
    parser_error(t, "Expected 'target' keyword");
    return 1;
  }

  parser_next(p);
  t = parser_peek(p);

  if (t.type != TOK_IDENTIFIER) {
    parser_error(t, "Expected target type after 'target'");
    return 1;
  }

  siex_target* target = arena_new_zero(p->memory, siex_target);
  

  if (strncmp(t.token_start, "embedded", t.length) == 0) {
    target->type = SIEX_TARGET_EMBEDDED;
  } else if (strncmp(t.token_start, "metal", t.length) == 0) {
    target->type = SIEX_TARGET_METAL;
  } else if (strncmp(t.token_start, "os", t.length) == 0) {
    target->type = SIEX_TARGET_OS;
  } else {
    parser_error(t, "Unknown target (expected embedded/metal/os)");
    return 1;
  }

  parser_next(p);
  t = parser_peek(p);

  vector_init_t(&target->default_sources, char*);
  vector_init_t(&target->default_includes, char*);

  if (t.type == TOK_SEMICOLON) {
    parser_next(p);
  }
  else if (t.type == TOK_LBRACE) {
    parser_next(p);

    while ((t = parser_peek(p)).type != TOK_RBRACE && t.type != TOK_EOF) {
      if (t.type == TOK_KEYWORD_SOURCES) {
        if (parse_target_sources_block(p, target) != 0)
          return 1;
      }
      else if (t.type == TOK_KEYWORD_INCLUDES) {
        if (parse_target_includes_block(p, target) != 0)
          return 1;
      }
      else {
        parser_error(t, "Unexpected token in target");
      }
    }
    if (t.type != TOK_RBRACE) {
      parser_error(t, "Expected '}' after target block");
      return 1;
    }

    parser_next(p);
    t = parser_peek(p);

    if (t.type != TOK_SEMICOLON) {
      parser_error(t, "Expected ';' after target declaration");
      return 1;
    }

    parser_next(p);
  }
  else {
    parser_error(t, "Expected ';' or '{' after target identifier");
    return 1;
  }



  

  return 0;
}

static int parse_target_sources_block(parser* p, siex_target* target) {
  if ((parse_block(p, target, TARGET_BLOCK, "sources")) != 0)
      return 1;
  return 0;
}

static int parse_target_includes_block(parser* p, siex_target* target) {
  if ((parse_block(p, target, TARGET_BLOCK, "includes")) != 0)
    return 1;
  return 0;
}
