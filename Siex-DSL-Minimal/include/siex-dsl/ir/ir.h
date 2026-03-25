#ifndef SIEX_DSL_IR_H
#define SIEX_DSL_IR_H

#include <stddef.h>
#include <siex-dsl/core/vector.h>
#include <siex-dsl/core/hashmap.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  const char* path; // normalize (absoluth path ideally)
  const char* dir; // base dir
} siex_file;

typedef enum {
  SIEX_TARGET_EMBEDDED,
  SIEX_TARGET_METAL,
  SIEX_TARGET_OS
} siex_target_type;

typedef struct {
  siex_target_type type;
  vector default_sources;
  vector default_includes;
} siex_target;

typedef enum {
  SIEX_ROOT_EXECUTABLE,
  SIEX_ROOT_LIBRARY,
  SIEX_ROOT_PLUGIN
} siex_root_type;

typedef struct {
  siex_root_type type;
  char* name;
  char* entry;
  char* version;
} siex_root;

typedef struct {
  char* path;
  char* alias; // can be NULL
} siex_import;


typedef struct {
  char* name;
} siex_impl;

typedef struct {
  char* name;

  vector implementations; // siex_strategy*
} siex_backend;

typedef struct {
  char* alias; // can be NULL
  char* name;
} siex_need;

typedef struct {
  char* name;

  vector sources; // char*
  vector includes; // char*
  vector needs;   // siex_need*
} siex_module;

typedef struct {
  char* module;
  char* backend;
  char* impl; // NULL posible
} siex_binding;

typedef struct {
  siex_file* file;
  siex_target target;
  siex_root root;

  hashmap aliases;

  vector imports;   // siex_import*
  vector modules;   // siex_module*
  vector backends;  // siex_backend*
  vector bindings;  // siex_binding*
} siex_program;

#ifdef __cplusplus
}
#endif

#endif
