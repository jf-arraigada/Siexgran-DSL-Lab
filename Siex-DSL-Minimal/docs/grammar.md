# Grammar for Siex DSL

The grammar proposed:

```ebnf
program         = { top_level_decl } ;

top_level_decl  =
      import_decl
    | root_decl
    | module_decl
    | backend_decl
    | bind_decl
    | target_decl
;

import_decl     =
    "import" string_literal [ "as" identifier ] ";"
;

root_decl       =
    "root" identifier "{"
        { root_stmt }
    "}" ";"
;

root_stmt       =
      root_type_decl
    | main_decl
    | version_decl
;

root_type_decl  =
    "type" ("executable" | "library" | "plugin") ";"
;

main_decl       =
    "main" identifier ";"
;

version_decl    =
    "version" string_literal ";"
;

module_decl     =
    "module" identifier "{"
        { module_stmt }
    "}" ";"
;

module_stmt     =
      sources_block
    | includes_block
    | need_decl       
;

need_decl       =
    "need" identifier [ "::" identifier ] ";"
;

backend_decl    =
    "backend" identifier (
        ";"
      | "{" { impl_decl } "}" ";"
    )
;

impl_decl       =
    "impl" identifier ";"
;

bind_decl       =
    "bind" identifier "=" bind_value ";"
;

bind_value      =
    identifier [ "." identifier ]
;

target_decl     =
    "target" target_type (
        ";"
      | "{" { target_stmt } "}" ";"
    )
;

target_type     =
      "embedded"
    | "metal"
    | "os"
;

target_stmt     =
      sources_block
    | includes_block
;

sources_block   =
    "sources" "{"
        { string_literal }
    "}"
;

includes_block  =
    "includes" "{"
        { string_literal }
    "}"
;

identifier      = /* IDENT */ ;
string_literal  = /* STRING */ ;
```

---
### Example of use:

```Siex
import "std/logger.siex" as log;

root app {
    type executable;
    main scheduler;
    version "0.3.0";
};

target os {
    sources {
        "absolute_path/to/sourceDir/"
    }
    includes {
        "relative_path/to/headerDir/"
    }
};

backend pq {
    impl heap;
    impl fifo;
};

backend ring;

module scheduler {
    need logger;
    need log::formatter; // To eliminate semantic ambiguities
};

module logger {
    sources {
        "path/to/dir/"
    }
};

bind scheduler = pq.heap;
bind logger = pq;
```

---
### Notes

- Imports:
An import may define an alias:
```Siex
import "file.siex" as alias;
```

### Need Statement

Supports optional aliasing:
```Siex
need module;
need alias::module;
```

### Binding

A binding may include an optional implementation:
```Siex
backend
backend.impl
```

### Target Types

Supported targets:
- embedded
- metal
- os

