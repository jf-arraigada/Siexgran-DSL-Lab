# Siexgran DSL Lab

Experimental laboratory for designing and implementing Domain-Specific Languages (DSLs) for the Siexgran ecosystem.

This repository explores language design, compiler architecture, and low-level tooling with a strong focus on performance, control, and minimal abstractions.

Multiple independent DSL implementations may coexist here, each representing a different stage in the evolution of the language.

## Structure

Each DSL version is contained in its own directory:

- `Siex-DSL-Minimal/` — Current experimental implementation
- (Future iterations will appear as the design evolves)

These versions are not necessarily backward compatible.  
They are intended as architectural experiments.

## Philosophy

- Zero-copy wherever possible
- Explicit over implicit behavior
- Minimal abstractions, maximum control
- Composable modules instead of monolithic systems
- Designed to integrate with Siexgran Platform

## Goals

- Experiment with zero-copy lexers
- Explore parser architectures (recursive descent, LL-style parsing)
- Design IR structures for modular systems
- Implement semantic analysis (resolver)
- Build dependency graphs for modular builds
- Iterate on DSL syntax and usability

## Current Status

### Siex-DSL-Minimal (v0.3.0)

- Zero-copy lexical analyzer
- Recursive descent parser
- Modular grammar with:
  - modules, backends, bindings, targets, root
- Support for:
  - `sources` and `includes` blocks
  - target-level default configuration
- Arena-based memory management
- Vector-based dynamic containers
- IR design for modular dependency systems
- Documentation aligned with grammar and implementation

### Next Steps

- Semantic resolver (dependency + binding validation)
- Build graph generation
- Code generation and backend integration
- Multi-file `.siex` support (modular composition)
- Improved diagnostics and developer experience

---

This repository is experimental by design.  
Expect iteration, refactoring, and architectural evolution.
