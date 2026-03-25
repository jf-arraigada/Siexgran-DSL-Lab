#ifndef SIEX_DSL_RESOLVED_IR_H
#define SIEX_DSL_RESOLVED_IR_H

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

#include <siex-dsl/core/vector.h>
#include <siex-dsl/core/symbol.h>
#include <siex-dsl/ir/ir.h>

typedef struct {
  siex_symbol_id id; // namespace del archivo

  vector default_sources;  // char*
  vector default_includes; // char*
} siex_resolved_target;

// -----------------------------

typedef struct {
  siex_symbol_id id;
  siex_program* owner;

  vector needs; // siex_resolved_module*
  vector sources;
  vector includes;

} siex_resolved_module;

// -----------------------------

typedef struct {
  const char* name;
} siex_resolved_impl;

typedef struct {
  siex_symbol_id id;

  vector impls; // siex_resolved_impl*
} siex_resolved_backend;

// -----------------------------

typedef struct {
  siex_resolved_module* module;
  siex_resolved_backend* backend;
  siex_resolved_impl* impl; // opcional
} siex_resolved_binding;

// -----------------------------

typedef struct {

  vector targets;    // siex_resolved_target*
  vector modules;   // siex_resolved_module*
  vector backends;  // siex_resolved_backend*
  vector bindings;  // siex_resolved_binding*
} siex_resolved_program;

#ifdef __cplusplus
}
#endif // !__cplusplus

#endif // !SIEX_DSL_RESOLVED_IR_H
