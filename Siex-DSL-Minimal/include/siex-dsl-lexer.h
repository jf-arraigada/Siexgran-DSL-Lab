#ifndef SIEX_DSL_LEXER_H
#define SIEX_DSL_LEXER_H

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

#include "siex-dsl-token.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

typedef struct lexer {
  const char* file;
  const char* start;
  const char* cursor;
  const char* end;
  source_location location;
} lexer;

int init_lexer(lexer* lex, const char* filename);
int free_lexer(lexer* lex);

char lexer_next_char(lexer* lex);
char lexer_peek_char(lexer* lex);

token lexer_next_token(lexer* lex);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !SIEX_DSL_LEXER_H
