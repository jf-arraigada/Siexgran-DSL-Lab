#ifndef SIEX_DSL_RESOLVER_INTERNAL_H
#define SIEX_DSL_RESOLVER_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

#include <siex-dsl/semantic/resolver.h>

int resolver_collect_modules(siex_resolver* resolver, siex_resolved_program* out);
int resolver_collect_backends(siex_resolver* resolver, siex_resolved_program* out);
int resolver_collect_targets(siex_resolver* resolver, siex_resolved_program* out);
int resolver_resolve_bindings(siex_resolver* resolver, siex_resolved_program* out);
int resolver_resolve_dependencies(siex_resolver* resolver, siex_resolved_program* out);
int resolver_apply_targets(siex_resolver* resolver, siex_resolved_program* out);

#ifdef __cplusplus
}
#endif // !__cplusplus

#endif // !SIEX_DSL_RESOLVER_INTERNAL_H
