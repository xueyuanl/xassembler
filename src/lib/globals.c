//
// Created by hsuehyuan li on 2020-04-05.
//

#include "globals.h"

//tables
LinkedList g_StringTable;
LinkedList g_FuncTable;
LinkedList g_SymbolTable;
LinkedList g_LabelTable;
LinkedList g_HostAPICallTable;
int g_iInstrTableLength;

Instr *g_pInstrStream = NULL;
int g_iInstrStreamSize;
int g_iCurrInstrIndex;

ScriptHeader g_ScriptHeader;

char **g_ppstrSourceCode;
int g_iInstrStreamSize = 0;
bool g_iIsSetStackSizeFound = FALSE;
int g_iSourceFileLine = 0;
char g_pstrExecFilename[MAX_SOURCE_LINE_SIZE] = "xas.out";
FILE *g_pSourceFile = NULL;

// The instruction lookup structure
Instruction g_InstrTable[MAX_INSTR_LOOKUP_COUNT];

// Token Stream
LinkedList g_TokenStream;
LinkedListNode *g_pCurrTokenNode = NULL;