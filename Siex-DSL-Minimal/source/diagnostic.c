#include <siex-dsl/diagnostic.h>
#include <stdio.h>

void diagnostic_report(FILE *out, diagnostic_type type, source_location location, const char *message) {
  const char* type_str = "";
  switch (type) {
    case DIAGNOSTIC_ERROR:
      type_str = "ERROR";
      break;
    case DIAGNOSTIC_WARNING:
      type_str = "WARNING";
      break;
    case DIAGNOSTIC_NOTE:
      type_str = "NOTE";
      break;
  }

  fprintf(out, "line: %zu column: %zu: [%s]: %s\n",
      location.line,
      location.column,
      type_str,
      message
      );
}

void diagnostic_error(source_location location, const char* message) {
  diagnostic_report(stderr, DIAGNOSTIC_ERROR, location, message);
}

void diagnostic_warning(source_location location, const char* message) {
  diagnostic_report(stdout, DIAGNOSTIC_WARNING, location, message);
}

void diagnostic_note(source_location location, const char* message) {
  diagnostic_report(stdout, DIAGNOSTIC_NOTE, location, message);
}
