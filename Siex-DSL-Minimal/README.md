## Siex DSL

### Version
Siex-Dsl-Minimal v0.4.0

Experimental declarative DSL for architecture and build graph definition, with zero-copy lexer, modular recursive descent parser, and multi-file support.

---

### Overview

Siex is a **declarative DSL** designed to describe:

- software modules
- backend implementations
- binding strategies
- dependency relationships
- system targets

It does **not express runtime logic**, but instead builds a **compile-time architecture graph**.

---

### Current Status

#### Frontend
- Zero-copy lexical analyzer
- Full keyword recognition:
  `root`, `module`, `backend`, `target`, `sources`, `includes`, `need`, `bind`, `impl`, `import`, `type`, `main`, `version`, `as`
- Token system with source location tracking (line/column)
- Diagnostic system:
  - errors
  - warnings
  - notes

#### Parser (Recursive Descent)
- Modular parser split by declaration type:
  - `root`
  - `module`
  - `backend`
  - `bind`
  - `target`
  - `import`
- Strict grammar enforcement (no backtracking)
- Single-token lookahead (`parser_peek`)
- Early error reporting

#### Language Features Implemented
- `root`:
  - `type` (executable / library / plugin)
  - `main`
  - `version`
- `module`:
  - `sources`
  - `includes`
  - `need` with alias support (`alias::module`)
- `backend`:
  - inline `impl` declarations
  - optional block or forward declaration
- `bind`:
  - `module = backend`
  - `module = backend.impl`
- `target`:
  - types: `embedded`, `metal`, `os`
  - optional block:
    - default `sources`
    - default `includes`
- `import`:
  - string path
  - optional alias (`as`)
  - designed for multi-file composition

#### IR (Intermediate Representation)
- Direct IR construction (no AST)
- Arena-based memory model
- Structures implemented:
  - `siex_program`
  - `siex_module`
  - `siex_backend`
  - `siex_impl`
  - `siex_binding`
  - `siex_target`
  - `siex_root`
  - `siex_import`

#### Core Systems
- Custom `arena` allocator (fast, linear allocation)
- Generic `vector` container (arena-backed)
- Robin Hood `hashmap` implementation
- Namespace system:
  - symbol identity: `file::name`
  - hashed via `siex_symbol_id`

#### Parser Internals
- Generic block parsing:
  - reused for `sources` and `includes`
  - shared between `module` and `target`
- Clean separation between:
  - public parser API
  - internal parsing logic

---

### Architecture (WIP Pipeline)

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

### Example

```Siex
import "std/logger.siex" as log;

root app {
    type executable;
    main scheduler;
    version "0.3.0";
};

target os {
    sources {
        "src/"
    }
    includes {
        "include/"
    }
};

backend pq {
    impl heap;
    impl fifo;
};

module scheduler {
    need logger;
    need log::formatter;
};

bind scheduler = pq.heap;
```

---

### Planned

#### Loader (in progress)
- Absolute path resolution
- Environment variable expansion
- Recursive import loading
- Deduplication via hashmap

#### Resolver (next major phase)
- Global symbol table construction
- Namespace resolution (`file::name`)
- Module resolution
- Backend / impl resolution
- Binding validation:
  - missing backend
  - missing implementation
- `need` dependency resolution
- Duplicate detection

#### Build Graph
- DAG construction from `need`
- Cycle detection
- Topological ordering

#### Code Generation
- Wrapper generation
- Backend dispatch glue
- Final project structure emission

#### Diagnostics
- Context-aware error messages
- Cross-file error reporting
- Improved developer feedback

---

### Design Principles

- No AST → direct semantic IR
- Arena allocation → performance & locality
- Deterministic parsing → predictable behavior
- Explicit namespaces → multi-file safety
- Minimal syntax → strong semantics

---

### Status Summary

Siex DSL is currently:

- **Parser-complete (v0.4.0)**
- Multi-file ready (import system defined)
- IR-stable for frontend
- Ready for resolver implementation

Next milestone:

> Semantic resolution + dependency graph
