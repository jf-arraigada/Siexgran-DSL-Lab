#include <siex-dsl/parser.h>
#include <siex-dsl/parser_internal.h>
#include <string.h>

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

  if (strncmp(t.token_start, "embedded", t.length) == 0) {
    p->program.target = SIEX_TARGET_EMBEDDED;
  } else if (strncmp(t.token_start, "metal", t.length) == 0) {
    p->program.target = SIEX_TARGET_METAL;
  } else if (strncmp(t.token_start, "os", t.length) == 0) {
    p->program.target = SIEX_TARGET_OS;
  } else {
    parser_error(t, "Unknown target (expected embedded/metal/os)");
    return 1;
  }

  parser_next(p);
  t = parser_peek(p);

  if (t.type != TOK_SEMICOLON) {
    parser_error(t, "Expected ';' after target declaration");
    return 1;
  }

  return 0;
}

