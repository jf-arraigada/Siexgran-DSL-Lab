cmake_minimum_required(VERSION 3.16)

project(Siex_Dsl_Minimal C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(siex_dsl
    source/vector.c
    source/lexer.c
    source/arena.c
    source/diagnostic.c
    source/parser/paser.c
    source/parser/parser_internal.c
    source/parser/decl/backend.c
    source/parser/decl/bind.c
    source/parser/decl/module.c
    source/parser/decl/root.c
    source/parser/decl/target.c
)

target_include_directories(siex_dsl
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(siex_dsl PUBLIC c_std_11)

