cmake_minimum_required(VERSION 3.13)
project(assembler C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-framework Python")
add_executable(assembler src/main.c src/lexer.c src/lexer.h src/lib/string_util.c src/table/func_table.c src/table/label_table.c src/table/string_table.c src/table/symbol_table.c)
