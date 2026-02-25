# Sigran DSL Lab

Experimental laboratory for designing and implementing Domain-Specific Languages (DSLs) for the Sigran ecosystem.

This repository is dedicated to exploring different language designs, lexer/parser architectures, and compiler construction strategies.  
Multiple independent DSL implementations may coexist here, each version representing a different design iteration or experiment.

## Structure

Each DSL version is contained in its own directory:

- `Siex-DSL-v-0.0.1/` — Initial zero-copy lexer implementation
- (Future versions will appear as the design evolves)

These versions are not necessarily backward compatible.  
They represent experimental stages and architectural exploration.

## Goals

- Experiment with zero-copy lexers
- Explore parser architectures (LL, recursive descent, etc.)
- Design AST/IR representations
- Evaluate different semantic analysis strategies
- Iterate over DSL syntax design

## Current Status

`Siex-DSL-v-0.0.1`:
- Zero-copy lexical analyzer
- Keyword recognition
- Token abstraction
- Basic project structure

Planned:
- Line/column tracking
- AST construction
- Semantic analysis phase

---

This repository is experimental by design.
Expect iteration, refactoring, and architectural evolution.
