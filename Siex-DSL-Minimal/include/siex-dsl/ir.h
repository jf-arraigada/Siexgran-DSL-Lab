#ifndef SIEX_DSL_IR_H
#define SIEX_DSL_IR_H

#include <stddef.h>
#include <siex-dsl/vector.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  SIEX_TARGET_EMBEDDED,
  SIEX_TARGET_METAL,
  SIEX_TARGET_OS
} siex_target;

typedef enum {
  SIEX_ROOT_EXECUTABLE,
  SIEX_ROOT_LIBRARY,
  SIEX_ROOT_PLUGIN
} siex_root_type;

typedef struct {
  char* name;
} siex_impl;

typedef struct {
  char* name;

  vector implementations; // siex_strategy*
} siex_backend;

typedef struct {
  char* name;

  vector sources; // char*
  vector needs;   // char*
} siex_module;

typedef struct {
  char* module;
  char* backend;
  char* service;
  char* impl; // NULL posible
} siex_binding;

typedef struct {
  char* root_name;

  siex_target target;
  siex_root_type root_type;

  vector modules;   // siex_module*
  vector backends;  // siex_backend*
  vector bindings;  // siex_binding*
} siex_program;

#ifdef __cplusplus
}
#endif

#endif
