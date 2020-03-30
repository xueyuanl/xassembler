//
// Created by hsuehyuan li on 2020-03-15.
//

#ifndef ASSEMBLER_INSTRUCTION_H
#define ASSEMBLER_INSTRUCTION_H

#include <string.h>
#include "../lib/constants.h"

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

int GetInstrByMnemonic(char *pstrMnemonic, Instruction *pInstr);

#endif //ASSEMBLER_INSTRUCTION_H¡1¡¡¡
