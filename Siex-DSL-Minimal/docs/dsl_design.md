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
       DSL source files
              ↓
   Loader (handles imports)
              ↓
 Per-file parsing (arena-based IR)
              ↓
         Merge phase
              ↓
   Resolver (symbol linking)
              ↓
 Dependency graph construction
              ↓
       Code generation
```

---

## Lexical Structure (Tokens)

The lexer produces the following categories of tokens.

### Keywords

- `root`
- `module`
- `backend`
- `target`
- `sources`
- `includes`
- `need`
- `bind`
- `impl`
- `import`
- `type`
- `main`
- `version`
- `as`

### Identifiers
`[a-zA-Z_][a-zA-Z0-9_]*`

Used for:

- root names
- module names
- backend names
- implementation names
- symbolic references

### Literals

- String literals `"..."`

### Punctuation

- `{` `}`
- `,`
- `;`
- `.`
- `=`
- `::`

### Comments

Single-line comments are supported:
```Siex
 // comment
```

Whitespace is ignored.

---

## Multi-File Architecture

Siex supports splitting configuration across multiple `.siex` files.

This is achieved via the `import` declaration:

```Siex
import "network.siex";
import "drivers.siex" as drv;
```

Each file is parsed independently into its own arena.

After parsing, all programs are merged and resolved globally.

### Key Properties

- Imports are recursive
- Import paths are resolved relative to the declaring file
- Aliases provide namespace disambiguation
- Duplicate loads are avoided via loader deduplication (`hashmap`)
- Symbols are globally visible after resolution

---

## Semantic Model (IR)

Siex does **not build a generic AST**.

Instead, parsing directly constructs a **semantic IR** using arenas.

This IR represents:

> a build graph + backend dispatch configuration

### Conceptual Structure

```
program
├── file (path, dir)
├── root
│   ├── type
│   ├── entry (main module)
│   └── version
├── target
│   ├── default_sources
│   └── default_includes
├── imports
├── modules
│   ├── sources
│   ├── includes
│   └── needs
├── backends
│   └── implementations
└── bindings
```

---

## Binding Model

Bindings define:

> module → backend[.impl]

Examples:

```Siex
bind scheduler = pq.heap;
bind logger = pq;
```

Meaning:

- A module is assigned a backend
- Optionally a specific implementation

---

## Namespace Model

Symbols are uniquely identified using:

```Siex
file::name
```

Example:

```Siex
logger.siex::formatter
```

This prevents collisions across imported files.

Internally:

- built via `namespace_build`
- hashed into `siex_symbol_id`

---

## Need Declaration (Alias Semantics)

```Siex
need module;
need alias::module;
```

Interpretation:

- `module` → same namespace
- `alias::module` → imported namespace

This avoids ambiguity across multiple imports.

---

## Target Model

```Siex
target embedded;
target os {
    sources { "..." }
    includes { "..." }
};
```

Supported:

- `embedded`
- `metal`
- `os`

Target provides:

- default sources
- default include paths

---

## Parser Architecture

The parser is:

- recursive descent
- single-token lookahead (`parser_peek`)
- arena-based allocation
- directly builds IR (no AST layer)

### Key Design Decisions

- No backtracking
- Strict grammar enforcement
- Early error reporting
- Minimal abstraction overhead

### Generic Block Parsing

`sources` and `includes` use a shared parser:

```C
parse_block(...)
```

This reduces duplication between:

- module blocks
- target blocks

---

## Post-Parsing Phases

### 1. Loader

Responsibilities:

- resolve absolute paths
- expand environment variables
- deduplicate loaded files
- recursively load imports

---

### 2. Resolver

Works globally across all programs.

Responsibilities:

- build symbol tables (using `hashmap`)
- assign `siex_symbol_id`
- resolve:
  - modules
  - backends
  - implementations
- validate bindings
- resolve `need` dependencies
- detect duplicates

---

### 3. Dependency Graph

Builds a DAG of modules:

- edges from `need`
- cycle detection
- topological ordering

---

### 4. Dispatch Strategy

Depends on target:

- embedded → static dispatch
- os → dynamic dispatch
- metal → configurable

---

## Code Generation Output

The compiler produces:

```
build/
 ├── wrapper_gen.h
 ├── modules/
 ├── backends/
 ├── bindings/
 └── final_project/
```

Contains:

- generated wrappers
- backend glue
- unified entry point
- copied sources

---

## Declaration Order

Top-level declarations are order-independent.

Valid:

```Siex
bind A = B.impl;

backend B {
    impl impl;
};

module A { };
```

Resolution is deferred to the resolver phase.

---

## Semantic Errors

Examples:

- undefined module in bind
- undefined backend
- missing implementation
- invalid alias reference
- duplicate symbols
- dependency cycles
- multiple root definitions

---

## Design Rationale

- No AST → simpler pipeline
- Arena allocation → fast & cache-friendly
- Explicit IR → easier codegen
- Namespaced symbols → multi-file safe
- Backend abstraction → flexible architecture
- Declarative model → predictable builds

---

## Summary

Siex DSL is:

> a build graph + architecture configuration language

Key properties:

- minimal syntax
- strong semantics
- multi-file aware
- backend-driven architecture
- compile-time validation

The frontend is:

- deterministic lexer
- recursive descent parser
- direct IR construction

This keeps complexity low while enabling powerful system design.
