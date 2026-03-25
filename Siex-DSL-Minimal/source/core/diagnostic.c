#include <siex-dsl/core/diagnostic.h>
#include <stdio.h>

void diagnostic_report(FILE *out, diagnostic_type type, const char *message) {
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

  fprintf(out, "[%s]: %s\n",
      type_str,
      message
      );
}

void diagnostic_report_location(FILE *out, diagnostic_type type, source_location location, const char *message) {
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
  diagnostic_report_location(stderr, DIAGNOSTIC_ERROR, location, message);
}

void diagnostic_warning(source_location location, const char* message) {
  diagnostic_report_location(stdout, DIAGNOSTIC_WARNING, location, message);
}

void diagnostic_note(source_location location, const char* message) {
  diagnostic_report_location(stdout, DIAGNOSTIC_NOTE, location, message);
}
