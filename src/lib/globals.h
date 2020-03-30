//
// Created by hsuehyuan li on 2020-03-15.
//

#ifndef ASSEMBLER_GLOBALS_H
#define ASSEMBLER_GLOBALS_H

#include <stdbool.h>
#include "linked_list.h"
#include "constants.h"
#include "../structure/instruction.h"

// Source code representation
char **g_ppstrSourceCode = NULL;
int g_iSourceCodeSize;
int g_iInstrStreamSize = 0;
bool g_iIsSetStackSizeFound = FALSE;


// The instruction lookup structure
Instruction g_InstrTable[MAX_INSTR_LOOKUP_COUNT];

typedef struct _Op              // An assembled operand
{
    int iType;                  // Type
    union                       // The value
    {
        int iIntLiteral;        // Integer literal
        float fFloatLiteral;    // Float literal
        int iStringTableIndex;  // String structure index
        int iStackIndex;        // Stack index
        int iInstrIndex;        // Instr index
        int iFuncIndex;         // Function index
        int iHostAPICallIndex;  // Host API Call index
        int iReg;               // Register code
    };
    int iOffsetIndex;           // Index of the offset
} Operand;

typedef struct _Instr           // An instruction
{
    int iOpCode;                // OpCode
    int iOpCount;               // Number of operands
    Operand *pOpList;           // Pointer to operand list
} Instr;
// The assembled instruction stream
Instr *g_pInstrStream = NULL;
int g_instrStreamSize;
int g_iCurrInstrIndex;


typedef struct _ScriptHeader    // Script header data
{
    int iStackSize;             // Requested stack size
    int iGlobalDataSize;        // The size of the script's global data
    int iIsMainFuncPresent;     // Is _Main() present?
    int iMainFuncIndex;         // _Main()'s function index
} ScriptHeader;
// The scrip header
ScriptHeader g_ScriptHeader;

// The main tables
LinkedList g_StringTable;
LinkedList g_FuncTable;
LinkedList g_SymbolTable;
LinkedList g_LabelTable;
LinkedList g_HostAPICallTable;

typedef int Token;

typedef struct _Lexer               // The lexer's state
{
    unsigned int iIndex0;
    unsigned int iIndex1;
    int iCurrSourceLine;
    int iCurrLexState;              // in string or not
    Token CurrToken;
    char pstrCurrLexeme[MAX_LEXEME_SIZE];

} Lexer;

Lexer g_Lexer;

#endif //ASSEMBLER_GLOBALS_H
