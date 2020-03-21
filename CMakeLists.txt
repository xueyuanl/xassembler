cmake_minimum_required(VERSION 3.13)
project(assembler C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-framework Python")
add_executable(assembler src/main.c src/lexer.c src/lexer.h src/lib/string_util.c src/structure/func_table.c src/structure/label_table.c src/structure/string_table.c src/structure/symbol_table.c)
