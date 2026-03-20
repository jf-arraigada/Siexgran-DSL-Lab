## Siex DSL

### Version
Siex-Dsl-Minimal v0.3.0

Experimental DSL with zero-copy lexer and modular recursive descent parser.

### Current Status
- Zero-copy lexical analyzer implemented
- Keyword recognition including `sources` and `includes`
- Token structure with line/column tracking
- Diagnostic system for errors, warnings, and notes
- Parser implemented with modular handling of:
  - root, module, backend, bind, target declarations
- Generic block parsing for shared constructs (`sources` / `includes`)
- Support for `includes` in both `module` and `target`
- Extended `target` declaration with optional block:
  - default sources
  - default includes
- Dynamic memory management via `arena`
- Dynamic arrays via `vector` allocated inside `arena`
- IR updated to support:
  - module includes
  - target default sources/includes
- Grammar and documentation updated to match implementation

### Planned
- Resolver / semantic analysis:
  - module dependency validation (`need`)
  - binding validation (module ↔ backend ↔ impl)
- Build graph construction phase
- Backend strategy resolution and default selection
- Wrapper and code generation
- Enhanced diagnostics:
  - contextual errors
  - recovery strategies
- Support for modular `.siex` composition (multi-file projects)
- Documentation expansion for resolver and build phases
