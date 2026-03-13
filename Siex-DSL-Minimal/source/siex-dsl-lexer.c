#include "../include/siex-dsl-lexer.h"
#include <string.h>
#include <ctype.h>

static char* read_file(const char* filename, size_t* file_size)
{
  FILE* file = fopen(filename, "r");

  if(!file) return NULL;
  
  // getting the file's size and returning the cursor to init state
  fseek(file, 0, SEEK_END);
  size_t size = ftell(file);
  rewind(file);

  char* buffer = malloc(size + 1);

  if(!buffer) {
    fclose(file);
    return NULL;
  }

  // getting the readed size to compare with file's size later
  size_t readed = fread(buffer, 1, size, file);
  fclose(file);

  buffer[size] = '\0';

  if(readed != size) {
    free(buffer);
    return NULL;
  }

  *file_size = size;
  return buffer;
}

static token make_token(token_type type, const char* start, size_t length, source_location location) {
  token t;
  t.type = type;
  t.token_start = start;
  t.length = length;
  t.location = location;
  return t;
}
static void skip_whitespace(lexer* lex) {
  for (;;) {

    char c = lexer_peek_char(lex);

    // comment
    if (c == '/' && lexer_peek_next(lex) == '/') {
      while (lexer_peek_char(lex) != '\n' &&
             lexer_peek_char(lex) != '\0') {
        lexer_next_char(lex);
      }
      continue;
    }

    // whitespace
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
      lexer_next_char(lex);
      continue;
    }

    break;
  }
}

static token_type check_keyword(const char* start, size_t length) {
  switch (length) {
    case 4:
      if (strncmp(start, "need", 4) == 0)
        return TOK_KEYWORD_NEED;
      if (strncmp(start, "bind", 4) == 0)
        return TOK_KEYWORD_BIND;
      break;

    case 6:
      if (strncmp(start, "module", 6) == 0)
        return TOK_KEYWORD_MODULE;
      if (strncmp(start, "target", 6) == 0)
        return TOK_KEYWORD_TARGET;
      break;

    case 7:
      if (strncmp(start, "backend", 7) == 0)
        return TOK_KEYWORD_BACKEND;
      if (strncmp(start, "sources", 7) == 0)
        return TOK_KEYWORD_SOURCES;
      break;
  }

  return TOK_IDENTIFIER;
}

int init_lexer(lexer *lex, const char *filename) {
  if(!lex || !filename) return 1;

  size_t file_size;
  lex->file = read_file(filename, &file_size);

  if(!lex->file) return 1;

  lex->cursor = lex->file;
  lex->start = lex->cursor;
  lex->end = lex->start + file_size;
  lex->location.line = 1;
  lex->location.column = 1;

  return 0;
}

int free_lexer(lexer *lex) {
  if(!lex) return 1;
  free((void*)lex->file);
  return 0;
}

char lexer_peek_char(lexer* lex) {
  if(!lex || lex->cursor >= lex->end) return '\0';
  return *lex->cursor;
}

char lexer_next_char(lexer *lex) {
  if(!lex || lex->cursor >= lex->end) return '\0';
  char c = *lex->cursor++;

  if(c == '\n') {
    lex->location.line++;
    lex->location.column = 1;
  }
  else {
    lex->location.column++;
  }

  return c;
}

char lexer_peek_next(lexer* lex) {
    if (!lex || lex->cursor + 1 >= lex->end) return '\0';
    return lex->cursor[1];
}


token lexer_next_token(lexer *lex) {
  skip_whitespace(lex);
  
  // NON EOF COMPROBATION
  if (lex->cursor >= lex->end) {
    return make_token(TOK_EOF, lex->cursor, 0, lex->location);
  }
  
  // TEMPORAL VARIABLES INITIALITION
  const char* start = lex->cursor;
  size_t length = 0;
  source_location token_location = lex->location;
  char c = lexer_peek_char(lex);

  // ONE-CHAR CASES
  switch (c) {
    case '{':
      lexer_next_char(lex);
      return make_token(TOK_LBRACE, start, 1, token_location);
    case '}':
      lexer_next_char(lex);
      return make_token(TOK_RBRACE, start, 1, token_location);
    case ',':
      lexer_next_char(lex);
      return make_token(TOK_COMMA, start, 1, token_location);
    case ';':
      lexer_next_char(lex);
      return make_token(TOK_SEMICOLON, start, 1, token_location);
    case '=':
      lexer_next_char(lex);
      return make_token(TOK_EQUAL, start, 1, token_location);
    case '.':
      lexer_next_char(lex);
      return make_token(TOK_DOT, start, 1, token_location);
    default:
      break;
  }

  // TOK_IDENTIFIER CASE
  if(isalpha(c) || c == '_') {
    lexer_next_char(lex);

    while (isalnum(lexer_peek_char(lex)) || lexer_peek_char(lex) == '_') {
      lexer_next_char(lex);
    }

    length = lex->cursor - start;

    token_type type = check_keyword(start, length);

    return make_token(type, start, length, token_location);
  }

  // TOK_NUMBER CASE
  if(isdigit(c)) {
    lexer_next_char(lex);

    while (isdigit(lexer_peek_char(lex))) {
      lexer_next_char(lex);
    }

    length = lex->cursor - start;
    return make_token(TOK_NUMBER, start, length, token_location);
  }

  // TOK_STRING CASE
  if(c == '"') {
    lexer_next_char(lex);

    while (lex->cursor < lex->end && lexer_peek_char(lex) != '"') {
      lexer_next_char(lex);
    }

    if (lexer_peek_char(lex) != '"') {
        return make_token(TOK_UNKNOWN, start, lex->cursor - start, token_location);
    }

    if(lex->cursor < lex->end) lexer_next_char(lex);
    
    length = lex->cursor - start;
    return make_token(TOK_STRING, start, length, token_location);
  }

  lexer_next_char(lex);
  return make_token(TOK_UNKNOWN, start, 1, lex->location);
}
