//
// Created by hsuehyuan li on 2020-03-15.
//

#ifndef ASSEMBLER_GLOBALS_H
#define ASSEMBLER_GLOBALS_H

#include <stdbool.h>
#include <stdio.h>
#include "linked_list.h"
#include "constants.h"
#include "../structure/instruction.h"
#include "../structure/token_stream.h"

// Source code representation
extern char **g_ppstrSourceCode;
extern int g_iSourceCodeSize;
extern int g_iInstrStreamSize;
extern bool g_iIsSetStackSizeFound;
extern int g_iSourceFileLine;
extern char g_pstrExecFilename[MAX_SOURCE_LINE_SIZE];
extern FILE *g_pSourceFile;

// The instruction lookup structure
extern Instruction g_InstrTable[MAX_INSTR_LOOKUP_COUNT];

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
extern Instr *g_pInstrStream;
extern int g_instrStreamSize;
extern int g_iCurrInstrIndex;


typedef struct _ScriptHeader    // Script header data
{
    int iStackSize;             // Requested stack size
    int iGlobalDataSize;        // The size of the script's global data
    int iIsMainFuncPresent;     // Is _Main() present?
    int iMainFuncIndex;         // _Main()'s function index
} ScriptHeader;
// The scrip header
extern ScriptHeader g_ScriptHeader;

// The main tables
extern LinkedList g_StringTable;
extern LinkedList g_FuncTable;
extern LinkedList g_SymbolTable;
extern LinkedList g_LabelTable;
extern LinkedList g_HostAPICallTable;

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

extern Lexer g_Lexer;

// Token Stream
extern LinkedList g_TokenStream;
extern LinkedListNode *g_pCurrTokenNode;

#endif //ASSEMBLER_GLOBALS_H
