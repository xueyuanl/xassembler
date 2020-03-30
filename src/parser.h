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

// ---- Assembled Instruction Stream ------------------------------------------------------
// ---- Operand Type Bitfield Flags ---------------------------------------------------
#define OP_TYPE_INT                 0           // Integer literal value
#define OP_TYPE_FLOAT               1           // Floating-point literal value
#define OP_TYPE_STRING_INDEX        2           // String literal value
#define OP_TYPE_ABS_STACK_INDEX     3           // Absolute array index
#define OP_TYPE_REL_STACK_INDEX     4           // Relative array index
#define OP_TYPE_INSTR_INDEX         5           // Instruction index
#define OP_TYPE_FUNC_INDEX          6           // Function index
#define OP_TYPE_HOST_API_CALL_INDEX 7           // Host API call index
#define OP_TYPE_REG                 8           // Register

// The following constants are used as flags into an operand type bit field, hence
// their values being increasing powers of 2.

#define OP_FLAG_TYPE_INT            1           // Integer literal value
#define OP_FLAG_TYPE_FLOAT          2           // Floating-point literal value
#define OP_FLAG_TYPE_STRING         4           // Integer literal value
#define OP_FLAG_TYPE_MEM_REF        8           // Memory reference (variable or array index, both absolute and relative)
#define OP_FLAG_TYPE_LINE_LABEL     16          // Line label (used for jumps)
#define OP_FLAG_TYPE_FUNC_NAME      32          // Function table index (used for Call)
#define OP_FLAG_TYPE_HOST_API_CALL  64          // Host API Call table index (used for CallHost)
#define OP_FLAG_TYPE_REG            128         // Register


// ---- Instruction Opcodes -----------------------------------------------------------

#define INSTR_MOV               0

#define INSTR_ADD               1
#define INSTR_SUB               2
#define INSTR_MUL               3
#define INSTR_DIV               4
#define INSTR_MOD               5
#define INSTR_EXP               6
#define INSTR_NEG               7
#define INSTR_INC               8
#define INSTR_DEC               9

#define INSTR_AND               10
#define INSTR_OR                11
#define INSTR_XOR               12
#define INSTR_NOT               13
#define INSTR_SHL               14
#define INSTR_SHR               15

#define INSTR_CONCAT            16
#define INSTR_GETCHAR           17
#define INSTR_SETCHAR           18

#define INSTR_JMP               19
#define INSTR_JE                20
#define INSTR_JNE               21
#define INSTR_JG                22
#define INSTR_JL                23
#define INSTR_JGE               24
#define INSTR_JLE               25

#define INSTR_PUSH              26
#define INSTR_POP               27

#define INSTR_CALL              28
#define INSTR_RET               29
#define INSTR_CALLHOST          30

#define INSTR_PAUSE             31
#define INSTR_EXIT              32

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
