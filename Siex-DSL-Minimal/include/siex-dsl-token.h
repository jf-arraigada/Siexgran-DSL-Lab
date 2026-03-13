#ifndef SIEX_DSL_TOKEN_H
#define SIEX_DSL_TOKEN_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "siex-dsl-common.h"

#include <stddef.h>
#include <stdlib.h>

typedef enum token_type {
  TOK_IDENTIFIER,
  TOK_STRING,
  TOK_NUMBER,
  TOK_EOF,
  TOK_UNKNOWN,

  TOK_KEYWORD_MODULE,
  TOK_KEYWORD_BACKEND,
  TOK_KEYWORD_NEED,
  TOK_KEYWORD_BIND,
  TOK_KEYWORD_TARGET,
  TOK_KEYWORD_SOURCES,


  TOK_LBRACE,
  TOK_RBRACE,
  TOK_COMMA,
  TOK_SEMICOLON,
  TOK_EQUAL,
  TOK_DOT,
} token_type;

typedef struct token {
  token_type type;
  const char* token_start;
  size_t length;
  source_location location;
} token;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SIEX_DSL_TOKEN_H
