# Siexgran DSL Lab

Experimental laboratory for designing and implementing Domain-Specific Languages (DSLs) for the Siexgran ecosystem.

This repository explores **language design, compiler architecture, and low-level tooling**, with a strong focus on performance, memory control, and explicit semantics.

Multiple DSL implementations may coexist here, each representing a different stage in the evolution of the system.

---

## Structure

Each DSL version is isolated:

- `Siex-DSL-Minimal/` — Current reference implementation (active development)
- Future iterations will evolve independently

These versions are **not guaranteed to be backward compatible**.  
They are intended as **architectural experiments and learning platforms**.

> The `build/` directory inside each module is ignored and not part of the design.

---

## Philosophy

- Zero-copy wherever possible
- Explicit semantics over implicit behavior
- Minimal abstractions, maximum control
- Arena-based memory for predictable allocation
- Composable modules instead of monolithic systems
- Strong separation between syntax, IR, and semantics

---

## Goals

- Design a declarative DSL for architecture and build systems
- Explore parser architectures (recursive descent, LL-style)
- Build a **direct semantic IR (no AST)**
- Implement a **multi-file aware compiler frontend**
- Develop a **resolver with global symbol linking**
- Construct a **dependency graph (DAG)**
- Enable backend-driven architecture and code generation

---

## Current Status

### Siex-DSL-Minimal (v0.4.0)

> Parser-complete, IR-stable, multi-file ready

#### Frontend

- Zero-copy lexical analyzer
- Full keyword support:
  - `root`, `module`, `backend`, `target`, `sources`, `includes`
  - `need`, `bind`, `impl`, `import`, `type`, `main`, `version`, `as`
- Token system with source location tracking
- Diagnostic system (errors, warnings, notes)

#### Parser

- Recursive descent (single-token lookahead)
- Strict grammar enforcement (no backtracking)
- Modular parsing by declaration:
  - `root`, `module`, `backend`, `bind`, `target`, `import`
- Generic block parser reused for:
  - `sources`
  - `includes`
- Direct IR construction (no AST layer)

#### Language Features

- `root`:
  - `type` (executable / library / plugin)
  - `main`
  - `version`
- `module`:
  - `sources`, `includes`
  - `need` with alias support (`alias::module`)
- `backend`:
  - forward declaration or block with `impl`
- `bind`:
  - `module = backend`
  - `module = backend.impl`
- `target`:
  - `embedded`, `metal`, `os`
  - default `sources` and `includes`
- `import`:
  - path-based
  - optional alias (`as`)

#### IR (Intermediate Representation)

- Arena-allocated
- No AST → direct semantic structures
- Core entities:
  - `siex_program`
  - `siex_module`
  - `siex_backend`
  - `siex_impl`
  - `siex_binding`
  - `siex_target`
  - `siex_root`
  - `siex_import`

#### Core Systems

- Custom arena allocator
- Generic vector (arena-backed)
- Robin Hood hashmap
- Namespace system:
  - symbol identity: `file::name`
  - hashed via `siex_symbol_id`

#### Multi-File Support (Infrastructure Ready)

- `import` system defined
- Loader API implemented:
  - path resolution
  - deduplication via hashmap
  - recursive loading (WIP integration)

---

## Architecture Pipeline (WIP)

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

## Documentation

Detailed design and grammar are available in:

- `Siex-DSL-Minimal/docs/dsl_design.md`
- `Siex-DSL-Minimal/docs/grammar.md`
- `Siex-DSL-Minimal/docs/grammar.ebnf`

These documents define:

- Formal grammar (EBNF)
- Semantic model (IR)
- Namespace system
- Binding and dependency rules
- Compiler architecture

---

## Next Steps

### In Progress

- Loader integration (full multi-file pipeline)

### Next Major Milestone

- Resolver implementation:
  - Global symbol table
  - Namespace resolution (`file::name`)
  - Backend / impl linking
  - Binding validation
  - Dependency resolution (`need`)
  - Duplicate detection

### Planned

- Dependency graph (cycle detection + topo sort)
- Target-aware dispatch strategy
- Code generation:
  - wrappers
  - backend glue
  - final project emission
- Cross-file diagnostics

---

## Design Direction

Siex is evolving toward:

> A declarative build graph + architecture configuration language

Key characteristics:

- Deterministic parsing
- Strong compile-time validation
- Multi-file composability
- Backend-driven modular design
- Explicit dependency modeling

---

## Notes

This repository is intentionally experimental.

Expect:

- Breaking changes
- Refactors
- Iterative design decisions

The goal is not stability, but **deep understanding and architectural clarity**.
