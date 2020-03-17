//
// Created by hsuehyuan li on 2020-03-15.
//

#ifndef ASSEMBLER_INSTRUCTION_H
#define ASSEMBLER_INSTRUCTION_H

#include <string.h>
#include "constants.h"

typedef int OpTypes;

typedef struct _InstrLookup     // An instruction lookup
{
    char pstrMnemonic
    [MAX_INSTR_MNEMONIC_SIZE];  // Mnemonic string
    int iOpCode;                // OpCode
    int iOpCount;               // Number of operands
    OpTypes *OpList;            // Pointer to operand list
} Instruction;

#endif //ASSEMBLER_INSTRUCTION_H
