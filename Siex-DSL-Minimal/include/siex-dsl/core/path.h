#ifndef SIEX_DSL_PATH_H
#define SIEX_DSL_PATH_H

#ifdef __cplusplus
extern "C" {
#endif // !__cplusplus

typedef struct {
    const char* raw;
    const char* resolved;
    const char* base_dir; // path del .siex que lo declaró
} siex_path;


#ifdef __cplusplus
}
#endif // !__cplusplus

#endif // !SIEX_DSL_PATH_H
