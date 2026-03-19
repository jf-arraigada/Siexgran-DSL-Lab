#include <siex-dsl/parser_internal.h>

token parser_peek(parser *p) {
  return p->current;
}

token parser_next(parser *p) {
  token t = p->current;
  p->current = lexer_next_token(p->lex);
  return t;
}

void parser_error(token t, const char* message) {
  diagnostic_error(t.location, message);
}

