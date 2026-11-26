cmake_minimum_required(VERSION 3.16)
project(rubro_negra_ed2 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (NOT MSVC)
    add_compile_options(-Wall -Wextra)
endif ()

# Biblioteca com a Arvore Rubro-Negra (include/ + src/)
add_library(rubronegra src/RedBlackTree.cpp)
target_include_directories(rubronegra PUBLIC include)

# Demonstracao passo a passo usada na videoaula
add_executable(demo_rb src/demo_rb.cpp)
target_link_libraries(demo_rb rubronegra)

# Testes automaticos (27 verificacoes)
add_executable(testes tests/testes.cpp)
target_link_libraries(testes rubronegra)
enable_testing()
add_test(NAME testes COMMAND testes)

# Exercicios: cada um e um arquivo unico, como seria enviado ao juiz
add_executable(ex1_validar_rubro_negra src/exercicios/ex1_validar_rubro_negra.cpp)
add_executable(ex2_insercao_fixup src/exercicios/ex2_insercao_fixup.cpp)
add_executable(ex3_remocao_fixup src/exercicios/ex3_remocao_fixup.cpp)
