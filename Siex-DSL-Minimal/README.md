## Siex DSL

### Version
Siex-Dsl-Minimal v0.2.0

Experimental DSL with zero-copy lexer and modular parser implementation.

### Current Status
- Zero-copy lexical analyzer implemented
- Keyword recognition and updated grammar
- Token structure defined with line/column tracking
- Minimal diagnostic features for parser and lexer
- Parser implemented with modular handling of:
  - root, module, backend, bind, target declarations
- Dynamic memory management via `arena` structure
- Dynamic arrays via `vector` allocated inside `arena`
- Initial `.siex` project example (`dummy.siex`)
- Documentation updated to match new grammar

### Planned
- Resolver / semantic analysis for dependency and binding validation
- Backend strategy resolution and default handling
- Wrapper and code generation
- Enhanced diagnostic messages with context and recovery
- Expanded documentation on resolver and code generation phases
