# DEPENDENCY STRATEGIES

### Backend Interface Model

A backend defines a concrete implementation of an abstract
operation set.

The dispatch strategy depends on the system target:

Metal / Embedded  → static dispatch (_Generic)
OS / Plugins      → dynamic dispatch (vtables)

### Static Dispatch (C11 _Generic)

Target:
- Bare Metal
- Embedded systems

Properties:
- Zero runtime overhead
- Compile-time dispatch
- Requires C11

**Example of backend:**
```C
struct pq_arena { ... };
struct pq_node  { ... };
```

**Example of DSL code gen:**
```C
// wrapper_gen.h
#define PQ_PUSH(q, v) PQ_PUSH_IMPL(q, v)

#define PQ_PUSH_IMPL(q, v) \
    _Generic((q), \
        pq_arena*:        pq_arena_push, \
        pq_node*:         pq_node_push, \
        struct pq_arena*: pq_arena_push, \
        struct pq_node*:  pq_node_push \
    )(q,v)
```
or
 
```C
// wrapper_gen.h

#define PQ_BACKENDS \
    X(pq_arena) \
    X(pq_node) \
    X(pq_ring) 

// PUSH SECTION

#define X(type) type*: type##_push,

#define pq_push(q, v) PQ_PUSH_IMPL(q ,v)

#define PQ_PUSH_IMPL(q, v) \
_Generic((q), \
    PQ_BACKENDS \
    )(q, v)

#undef X
```

**Example of use:**
```
PQ_PUSH(&arena_q, v);
PQ_PUSH(&node_q, v);
```

***Case of use:*** When system_target (Siex DSL definition) is Metal (or Bare Metal)

---

### Dynamic Dispatch (Vtables)


Target:
- OS environments
- Plugin systems

Properties:
- Runtime backend selection
- Supports plugins
- Small indirection overhead

**Example of Backend:**

```C
// scheduler.h
#include "priority_queue_backend.h"

struct scheduler {
    scheduler_impl impl;
    ...
};

// priority_queue_backend.h
#include "priority_queue_backend_ops.h"

struct priority_queue {
    ...
    struct pq_ops* ops;
};

// priority_queue_backend_ops.h
#include "wrapper_gen.h"

struct pq_ops {
    void (*push)(struct priority_queue* this, task T);
    task (*pull)(struct priority_queue* this);
};
```

**Example of DSL gen code:**
```C
// wrapper_gen.h

#define scheduler_impl scheduler_pq_ops

struct pq_ops scheduler_pq_ops = {
    .push = pq_heap_push,
    .pop  = pq_heap_pop
};

struct pq_ops notifier_pq_ops = {
    .push = pq_arena_push,
    .pop  = pq_arena_pop
};

static inline void pq_push(struct priority_queue* q, task t)
{
    q->ops->push(q, t);
}
```

**Example of use:**
```C
// My_module.c
...
scheduler scheduler_q;
notifier notifier_q;
task t;
...
pq_push(&scheduler_q, task);
pq_push(&notifier_q, task);
```

***Case of use:*** When system_target (Siex DSL definition) is any OS (Windows, Linux, etc)

