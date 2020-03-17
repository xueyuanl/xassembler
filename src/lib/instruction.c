//
// Created by hsuehyuan li on 2020-03-15.
//

#include "instruction.h"
#include "globals.h"

void strupr(char *string) {
    while (*string) {
        if (*string >= 'a' && *string <= 'z') {
            *string = *string - 32;
        }
        string++;
    }
}

int AddInstrLookup(char *pstrMnemonic, int iOpCode, int iOpCount) {
    // Just use a simple static int to keep track of the next instruction index in the table
    static int iInstrIndex = 0;

    // Make sure we haven't run out of instruction indices
    if (iInstrIndex >= MAX_INSTR_LOOKUP_COUNT)
        return -1;

    // Set the mnemonic, opcode and operand count fields
    strcpy(g_InstrTable[iInstrIndex].pstrMnemonic, pstrMnemonic);
    strupr(g_InstrTable[iInstrIndex].pstrMnemonic);
    g_InstrTable[iInstrIndex].iOpCode = iOpCode;
    g_InstrTable[iInstrIndex].iOpCount = iOpCount;

    // Allocate space for the operand list
    g_InstrTable[iInstrIndex].OpList = (OpTypes *) malloc(iOpCount * sizeof(OpTypes));

    // Copy the instruction index into another variable so it can be returned to the caller
    int iReturnInstrIndex = iInstrIndex;

    // Increment the index for the next instruction
    ++iInstrIndex;

    // Return the used index to the caller
    return iReturnInstrIndex;
}

void SetOpType(int iInstrIndex, int iOpIndex, OpTypes iOpType) {
    g_InstrTable[iInstrIndex].OpList[iOpIndex] = iOpType;
}

int GetInstrByMnemonic(char *pstrMnemonic, Instruction *pInstr) {
    // Loop through each instruction in the lookup table
    for (int iCurrInstrIndex = 0; iCurrInstrIndex < MAX_INSTR_LOOKUP_COUNT; ++iCurrInstrIndex) {
        // Compare the instruction's mnemonic to the specified one
        if (strcmp(g_InstrTable[iCurrInstrIndex].pstrMnemonic, pstrMnemonic == 0)) {
            // Set the instruction definition to the user-specified pointer
            *pInstr = g_InstrTable[iCurrInstrIndex];
            return TRUE;
        }
        // A match was not found, so return FALSE
        return FALSE;

    }
}
