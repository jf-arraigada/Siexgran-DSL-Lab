#ifndef SIEX_DSL_DIAGNOSTIC_H
#define SIEX_DSL_DIAGNOSTIC_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <siex-dsl/common.h>  
#include <stdio.h>

typedef enum diagnostic_type {
  DIAGNOSTIC_ERROR,
  DIAGNOSTIC_WARNING,
  DIAGNOSTIC_NOTE,
} diagnostic_type;

typedef struct diagnostic {
  diagnostic_type type;
  source_location location;
  const char* message;
} diagnostic;

//diagnostic make_diagnostic_message(source_location location, const char* message);
//void diagnostic_report(diagnostic* diag);
void diagnostic_report(FILE* out, diagnostic_type type, source_location location, const char* message);
void diagnostic_error(source_location location, const char* message);
void diagnostic_warning(source_location location, const char* message);
void diagnostic_note(source_location location, const char* message);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !SIEX_DSL_DIAGNOSTIC_H
