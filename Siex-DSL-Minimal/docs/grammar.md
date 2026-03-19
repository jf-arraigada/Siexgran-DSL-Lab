# Grammar for Siex DSL

The grammar proposed:
```ebnf
program         = { top_level_decl } ;

top_level_decl  =
      module_decl
    | backend_decl
    | target_decl
    | bind_decl
    | root_decl
;

root_decl       =
    "root" identifier "=" ("executable" | "library" | "plugin") ";"
;

module_decl     =
    "module" identifier "{"
        { module_stmt }
    "}" ";"
;

module_stmt     =
      need_decl
    | sources_block
;

need_decl       =
    "need" identifier ";"
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
    "bind" bind_target "=" bind_value ";"
;

bind_target     =
    identifier "." identifier
;

bind_value      =
    identifier [ "." identifier ]
;

target_decl     =
    "target" identifier ";"
;

sources_block   =
    "sources" "{"
        { string_literal }
    "}"
;

identifier      = /* IDENT */ ;
string_literal  = /* STRING */ ;
```


Example of use
```Siex

root My_app = executable;

backend pq;
backend ring;

module scheduler {
    need queue;
    need timer;
}

module logger {
    need queue;
}

bind scheduler.queue = pq.heap;
bind scheduler.timer = ring;

bind logger.queue = pq.intrusive;

target linux;
```

A binding right-hand side may include an optional strategy:

`backend.strategy`

The `need` statement declares that a module requires
an abstract service which must be satisfied by a backend
binding at the program level.
