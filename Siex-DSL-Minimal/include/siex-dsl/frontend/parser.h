#ifndef SIEX_DSL_PARSER_H
#define SIEX_DSL_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <siex-dsl/core/diagnostic.h>
#include <siex-dsl/core/arena.h>
#include <siex-dsl/frontend/lexer.h>
#include <siex-dsl/frontend/token.h>
#include <siex-dsl/ir/ir.h>

typedef struct parser {
    lexer* lex;
    arena* memory;
    token current;
    siex_program program;
} parser;

// Parser API
int init_parser(parser* p, lexer* lex);
token parser_peek(parser* p);
token parser_next(parser* p);

int parse_program(parser* p);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !SIEX_DSL_PARSER_H

