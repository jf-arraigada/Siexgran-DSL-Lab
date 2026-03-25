#include <siex-dsl/frontend/parser.h>
#include <siex-dsl/frontend/parser_internal.h>
#include <string.h>

static int parse_root_type(parser* p, siex_root* root);
static int parse_main_decl(parser* p, siex_root* root);
static int parse_version_decl(parser* p, siex_root* root);

int parse_root_decl(parser *p) {
  token t = parser_peek(p);
  
  if(t.type != TOK_KEYWORD_ROOT) {
    parser_error(t, "Expected 'root' keyword");
    return 1;
  }

  parser_next(p);
  t = parser_peek(p);

  if (t.type != TOK_IDENTIFIER) {
    parser_error(t, "Expected root name");
    return 1;
  }

  char* name = arena_strdup(p->memory, t.token_start, t.length);
  
  parser_next(p);
  t = parser_peek(p);

  if (t.type != TOK_LBRACE) {
    parser_error(t, "Expected '{' after root name");
    return 1;
  }

  parser_next(p);

  siex_root* root = arena_new_zero(p->memory, siex_root);
  root->name = name;

  while ((t = parser_peek(p)).type != TOK_RBRACE && t.type != TOK_EOF) {

    switch (t.type) {

      case TOK_KEYWORD_TYPE:
        if (parse_root_type(p, root) != 0)
          return 1;
        break;

      case TOK_KEYWORD_MAIN:
        if (parse_main_decl(p, root) != 0)
          return 1;
        break;

      case TOK_KEYWORD_VERSION:
        if (parse_version_decl(p, root) != 0)
          return 1;
        break;

      default:
        parser_error(t, "Unexpected token in root");
        return 1;
    }
  }
  if (t.type != TOK_RBRACE) {
    parser_error(t, "Expected '}' after root block");
    return 1;
  }

  parser_next(p);

  if (parser_peek(p).type != TOK_SEMICOLON) {
    parser_error(parser_peek(p), "Expected ';' after root block");
    return 1;
  }

  parser_next(p);

  return 0;
}

static int parse_root_type(parser* p, siex_root* root) {
  token t = parser_peek(p);

  if (t.type != TOK_KEYWORD_TYPE) {
    parser_error(t, "Expected 'type' keyword in root");
    return 1;
  }

  parser_next(p);
  t = parser_peek(p);

  if (t.type != TOK_IDENTIFIER) {
    parser_error(t, "Expected root type after 'type'");
    return 1;
  }
  
  if (strncmp(t.token_start, "executable", t.length) == 0) {
    root->type = SIEX_ROOT_EXECUTABLE;
  }
  else if (strncmp(t.token_start, "library", t.length) == 0) {
    root->type = SIEX_ROOT_LIBRARY;
  }
  else if (strncmp(t.token_start, "plugin", t.length) == 0) {
    root->type = SIEX_ROOT_PLUGIN;
  }
  else {
    parser_error(t, "Unknown root type");
  }

  parser_next(p);
  t = parser_peek(p);

  if (t.type != TOK_SEMICOLON) {
    parser_error(t, "Expected ';' after 'type' declaration");
    return 1;
  }

  parser_next(p);

  return 0;
}

static int parse_main_decl(parser* p, siex_root* root) {
  token t = parser_peek(p);

  if (root->entry != NULL) {
    parser_error(t, "Multiple 'main' declarations");
    return 1;
  }

  if (t.type != TOK_KEYWORD_MAIN) {
    parser_error(t, "Expected 'main' declaration in root");
    return 1;
  }

  parser_next(p);
  t = parser_peek(p);

  if (t.type != TOK_IDENTIFIER) {
    parser_error(t, "Expected module name after 'main'");
    return 1;
  }

  char* main = arena_strdup(p->memory, t.token_start, t.length);
  root->entry = main;

  parser_next(p);
  t = parser_peek(p);

  if (t.type != TOK_SEMICOLON) {
    parser_error(t, "Expected ';' after main declaration");
    return 1;
  }

  parser_next(p);

  return 0;
}

static int parse_version_decl(parser* p, siex_root* root) {
  token t = parser_peek(p);

  if (t.type != TOK_KEYWORD_VERSION) {
    parser_error(t, "Expected 'version' keyword in root");
    return 1;
  }

  parser_next(p);
  t = parser_peek(p);

  if (t.type != TOK_STRING) {
    parser_error(t, "Expected number after 'version'");
    return 1;
  }

  char* version = arena_strdup(p->memory, t.token_start, t.length);
  root->version = version;

  parser_next(p);
  t = parser_peek(p);

  if (t.type != TOK_SEMICOLON) {
    parser_error(t, "Expected ';' after version declaration");
    return 1;
  }

  parser_next(p);

  return 0;
}
