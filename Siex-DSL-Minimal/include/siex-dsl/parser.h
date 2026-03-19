#ifndef SIEX_DSL_PARSER_H
#define SIEX_DSL_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <siex-dsl/diagnostic.h>
#include <siex-dsl/arena.h>
#include <siex-dsl/lexer.h>
#include <siex-dsl/token.h>
#include <siex-dsl/ir.h>

typedef struct parser {
    lexer* lex;
    token current;
    arena mem;
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

