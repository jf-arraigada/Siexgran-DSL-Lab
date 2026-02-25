#ifndef SIEX_DSL_TOKEN_H
#define SIEX_DSL_TOKEN_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stddef.h>
#include <stdlib.h>

typedef enum {
  TOK_IDENTIFIER,
  TOK_STRING,
  TOK_NUMBER,
  TOK_EOF,
  TOK_UNKNOWN,

  TOK_KEYWORD_CONTRACT,
  TOK_KEYWORD_MODULE,
  TOK_KEYWORD_ROOT,
  TOK_KEYWORD_BACKEND,
  TOK_KEYWORD_SOURCE,
  TOK_KEYWORD_USE,
  TOK_KEYWORD_IMPLEMENT,
  TOK_KEYWORD_TYPE,

  TOK_LBRACE,
  TOK_RBRACE,
  TOK_COLON,
  TOK_SEMICOLON,
  TOK_EQUAL,
} token_type;

typedef struct token {
  token_type type;
  const char* token_start;
  size_t length;
} token;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // SIEX_DSL_TOKEN_H
