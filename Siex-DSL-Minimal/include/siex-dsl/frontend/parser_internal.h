#ifndef SIEX_DSL_PARSER_INTERNAL_H
#define SIEX_DSL_PARSER_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

#include <siex-dsl/frontend/parser.h>

typedef enum {
  MODULE_BLOCK,
  TARGET_BLOCK,
} block_type;

token parser_peek(parser *p);
token parser_next(parser *p);
void parser_error(token t, const char* msg);


// forward declarations
int parse_root_decl(parser *p);
int parse_import_decl(parser* p);
int parse_module_decl(parser *p);
int parse_backend_decl(parser *p);
int parse_bind_decl(parser *p);
int parse_target_decl(parser *p);
int parse_block(parser* p, void* block, block_type type, const char* keyword);

#ifdef __cplusplus
}
#endif // !__cplusplus

#endif // !SIEX_DSL_PARSER_INTERNAL_H
