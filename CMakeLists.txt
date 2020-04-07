cmake_minimum_required(VERSION 3.13)
project(assembler C)

set(CMAKE_C_STANDARD 99)
# set(CMAKE_C_FLAGS "-framework Python")
add_executable(assembler
        src/lib/string_util.c
        src/lib/string_util.h
        src/lib/constants.h
        src/lib/error.c
        src/lib/error.h
        src/lib/globals.c
        src/lib/globals.h
        src/lib/linked_list.c
        src/lib/linked_list.h
        src/structure/func_table.c
        src/structure/func_table.h
        src/structure/label_table.c
        src/structure/label_table.h
        src/structure/string_table.c
        src/structure/string_table.h
        src/structure/symbol_table.c
        src/structure/symbol_table.h
        src/structure/instruction.c
        src/structure/instruction.h
        src/lexer.c
        src/lexer.h
        src/parser.c
        src/parser.h
        src/main.c src/structure/token_stream.c src/structure/token_stream.h)
