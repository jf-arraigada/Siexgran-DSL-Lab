#include <siex-dsl/frontend/parser_internal.h>
#include <stdio.h>
#include <string.h>

#define BLOCK_SOURCES(b, t) ((t) == MODULE_BLOCK ? &((siex_module*)(b))->sources : &((siex_target*)(b))->default_sources)
#define BLOCK_INCLUDES(b, t) ((t) == MODULE_BLOCK ? &((siex_module*)(b))->includes : &((siex_target*)(b))->default_includes)


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

static void parser_error_args(token t, const char* message, const char* args) {
  char buffer[128];
  snprintf(buffer, sizeof(buffer), message, args);
  parser_error(t, buffer);
}

int parse_block(parser* p, void* block, block_type type, const char* keyword) {
  vector* sources_vec = BLOCK_SOURCES(block, type);
  vector* includes_vec = BLOCK_INCLUDES(block, type);

  token t = parser_peek(p);

  if (t.type != TOK_KEYWORD_MODULE && t.type != TOK_KEYWORD_TARGET) {
    parser_error_args(t, "Expected '%s' keyword", keyword);
    return 1;
  }

  parser_next(p);
  t = parser_peek(p);

  if (t.type != TOK_LBRACE) {
    parser_error_args(t, "Expected '{' after %s declaration", keyword);
    return 1;
  }

  parser_next(p);

  while((t = parser_peek(p)).type != TOK_RBRACE && t.type != TOK_EOF) {
    if (t.type != TOK_STRING) {
      parser_error_args(t, "Expected string literal in %s", keyword);
      return 1;
    }

    char* string = arena_strdup(p->memory, t.token_start + 1, t.length - 2);
    parser_next(p);

    if ((strncmp(keyword, "sources", 7)) == 0) {
      vector_push(p->memory, sources_vec, &string);
    }
    else if ((strncmp(keyword, "includes", 8)) == 0) {
      vector_push(p->memory, includes_vec, &string);
    }

  }

  if (t.type != TOK_RBRACE) {
    parser_error_args(t, "Expected '}' after %s block", keyword);
    return 1;
  }

  parser_next(p);

  return 0;
}
