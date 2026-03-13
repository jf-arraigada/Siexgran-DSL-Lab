# Siex DSL – Frontend & Build Graph Design

This document describes the frontend architecture of the Siex DSL,
including lexical structure, semantic model (build graph),
and backend dependency strategy overview.

---

## Overview

Siex is a **declarative architecture configuration DSL**.

Its purpose is NOT to express runtime logic but to describe:

- software modules
- implementation backends
- dispatch strategies
- dependency relationships
- system target constraints

The compiler pipeline is:

```
    DSL source
    ↓
    Lexer
    ↓
    Parser
    ↓
    Semantic Model (Build Graph / IR)
    ↓
    Resolver & Planner
    ↓
    Code Generator
    ↓
    Intermediate Build Directory
    ↓
    External Build Tool
```

---

## Lexical Structure (Tokens)

The lexer produces the following categories of tokens.

### Keywords

- `module`
- `backend`
- `target`
- `sources`
- `need`
- `bind`

### Identifiers
*[a-zA-Z_][a-zA-Z0-9_]*

Used for:

- module names
- backend names
- strategy names
- symbolic references

### Literals

- Integer numbers
- String literals `"..."`

### Punctuation

- `{` `}`
- `,`
- `;`
- `.`
- `=`

### Comments

Single-line comments are supported:
```Siex
//comment
```
Whitespace is ignored.

---

## Semantic Model (Build Graph)

Unlike traditional compilers, Siex does **not require a generic AST**.

Instead, parsing directly constructs a **semantic intermediate representation (IR)**,
which models the architecture of the system.

This IR is effectively:

> a build graph + backend dispatch configuration graph.

### Conceptual Structure
```
program
├── target
├── modules
│   ├── sources
│   └── dependencies
├── backends
│   └── strategies
└── bindings
```

### Key Relation

Bindings form a critical triple:
**module → backend → strategy**


This defines how each module obtains a concrete implementation.

### Abstract Services

A `need` declaration introduces an abstract service dependency.

Services form a logical namespace local to each module.

Example:
```Siex
    module scheduler {
        need queue;
    }
```
This means the module requires a service named `queue`,
which must later be satisfied by a binding.

Bindings connect:

    module.service → backend.strategy

---

## Suggested C Representation

```c
typedef struct {
    char* name;
} siex_strategy;

typedef struct {
    char* name;

    siex_strategy* strategies;
    size_t strategy_count;
} siex_backend;

typedef struct {
    char* name;

    char** sources;
    size_t source_count;

    char** needs;
    size_t need_count;
} siex_module;

typedef struct {
    char* module;
    char* backend;
    char* strategy;
} siex_binding;

typedef enum {
    SIEX_TARGET_METAL,
    SIEX_TARGET_OS
} siex_target;

typedef struct {
    siex_target target;

    siex_module* modules;
    size_t module_count;

    siex_backend* backends;
    size_t backend_count;

    siex_binding* bindings;
    size_t binding_count;
} siex_program;
```

This structure represents the semantic IR of the DSL.
You can see more about the grammar of Siex DSL on:
[Grammar](./grammar.md)


---
## Post-Parsing Phases

After parsing, the compiler performs:

### Name Resolution 

- resolve module references

- resolve backend references

- resolve strategies

- detect undefined symbols

### Dependency Graph Construction 

- build module dependency DAG

- detect cycles

- compute build order

### Dispatch Strategy Selection 

Based on system target:

- static dispatch for embedded targets

- dynamic dispatch for OS targets

See:
[Dependency Strategy](./dependency_strategy.md)

---
## Code Generation Output
The compiler generates an **intermediate build directory**:

```
build/
 ├── wrapper_gen.h
 ├── modules/
 ├── backends/
 ├── generated_bindings/
 └── final_project/
 ```
> [!INFO]
> The final_project directory may contain:
>
> - copied sources
> - generated wrappers
> - backend glue code
> - unified build entry point

This directory can then be consumed by an external build system
(e.g. Make, Ninja, CMake, custom tool).

---
## Declaration Order

Top-level declarations are order-independent.

Bindings may reference modules and backends declared later.
Name resolution is performed after parsing.
This implies the parser stores symbolic references without resolving them immediately.

A dedicated resolution phase validates and links all entities.

---
## Semantic Errors

The compiler may report errors such as:

- binding to an undefined module
- binding to an undefined backend
- unknown strategy
- unsatisfied module service
- dependency cycles between modules
- incompatible backend for selected target

---
## Design Rationale

Key properties of this architecture:

- No runtime DSL interpreter required

- Backend polymorphism resolved via code generation

- Supports static or dynamic dispatch

- Enables architecture validation before build

- Scales to embedded and OS targets

- Keeps modules backend-agnostic

---
## Summary

Siex DSL acts as:

> a software architecture configurator and build graph generator.

Its frontend is intentionally simple:

- deterministic lexer

- recursive-descent parser

- direct semantic model construction

This design reduces compiler complexity while enabling powerful
code generation and system configuration capabilities.
