//
// Created by hsuehyuan li on 2020-03-28.
//

#ifndef ASSEMBLER_PARSER_H
#define ASSEMBLER_PARSER_H

#include "lib/globals.h"
#include "structure/func_table.h"
#include "structure/instruction.h"
#include "lexer.h"
#include "lib/error.h"
#include "structure/func_table.h"
#include "structure/symbol_table.h"
#include "structure/label_table.h"
#include "structure/string_table.h"
#include "structure/token_stream.h"



// ---- Error Strings ---------------------------------------------------------------------
// The following macros are used to represent assembly-time error strings

#define ERROR_MSSG_INVALID_INPUT    \
    "Invalid input"

#define ERROR_MSSG_LOCAL_SETSTACKSIZE    \
    "SetStackSize can only appear in the global scope"

#define ERROR_MSSG_INVALID_STACK_SIZE    \
    "Invalid stack size"

#define ERROR_MSSG_MULTIPLE_SETSTACKSIZES    \
    "Multiple instances of SetStackSize illegal"

#define ERROR_MSSG_IDENT_EXPECTED    \
    "Identifier expected"

#define ERROR_MSSG_INVALID_ARRAY_SIZE    \
    "Invalid array size"

#define ERROR_MSSG_IDENT_REDEFINITION    \
    "Identifier redefinition"

#define ERROR_MSSG_UNDEFINED_IDENT    \
    "Undefined identifier"

#define ERROR_MSSG_NESTED_FUNC    \
    "Nested functions illegal"

#define ERROR_MSSG_FUNC_REDEFINITION    \
    "Function redefinition"

#define ERROR_MSSG_UNDEFINED_FUNC    \
    "Undefined function"

#define ERROR_MSSG_GLOBAL_PARAM    \
    "Parameters can only appear inside functions"

#define ERROR_MSSG_MAIN_PARAM    \
    "_Main () function cannot accept parameters"

#define ERROR_MSSG_GLOBAL_LINE_LABEL    \
    "Line labels can only appear inside functions"

#define ERROR_MSSG_LINE_LABEL_REDEFINITION    \
    "Line label redefinition"

#define ERROR_MSSG_UNDEFINED_LINE_LABEL    \
    "Undefined line label"

#define ERROR_MSSG_GLOBAL_INSTR    \
    "Instructions can only appear inside functions"

#define ERROR_MSSG_INVALID_INSTR    \
    "Invalid instruction"

#define ERROR_MSSG_INVALID_OP    \
    "Invalid operand"

#define ERROR_MSSG_INVALID_STRING    \
    "Invalid string"

#define ERROR_MSSG_INVALID_ARRAY_NOT_INDEXED    \
    "Arrays must be indexed"

#define ERROR_MSSG_INVALID_ARRAY    \
    "Invalid array"

#define ERROR_MSSG_INVALID_ARRAY_INDEX    \
    "Invalid array index"

void parse();

#endif //ASSEMBLER_PARSER_H
