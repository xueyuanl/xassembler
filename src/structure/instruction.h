//
// Created by hsuehyuan li on 2020-03-15.
//

#ifndef ASSEMBLER_INSTRUCTION_H
#define ASSEMBLER_INSTRUCTION_H

#include <string.h>
#include "../lib/constants.h"
#include "../lib/string_util.h"

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


typedef int OpTypes;

typedef struct _InstrLookup     // An instruction lookup
{
    char pstrMnemonic
    [MAX_INSTR_MNEMONIC_SIZE];  // Mnemonic string
    int iOpCode;                // OpCode
    int iOpCount;               // Number of operands
    // A dynamic array of 4-byte(int) bitfields,
    // each of which contains a series of 1-bit flags that
    // determine which data types the corresponding operand can accept.
    OpTypes *OpList;            // Pointer to operand list
} Instruction;

int AddInstrLookup(char *pstrMnemonic, int iOpCode, int iOpCount);

void SetOpType(int iInstrIndex, int iOpIndex, OpTypes iOpType);

int GetInstrByMnemonic(char *pstrMnemonic, Instruction *pInstr);

void InitInstrTable();

#endif //ASSEMBLER_INSTRUCTION_H
