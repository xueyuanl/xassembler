//
// Created by hsuehyuan li on 2020-03-15.
//

#ifndef ASSEMBLER_SYMBOL_TABLE_H
#define ASSEMBLER_SYMBOL_TABLE_H

#include <stdlib.h>
#include <string.h>
#include "../lib/constants.h"
#include "../lib/linked_list.h"
#include "../lib/globals.h"

typedef struct _SymbolNode          // A symbol table node
{
    int iIndex;                     // Index
    char pstrIdent
    [MAX_IDENT_SIZE];               // Identifier
    int iSize;                      // Size (1 for variables, N for arrays)
    int iStackIndex;                // the stack index to which the symbol points
    int iFuncIndex;                 // Function in which the symbol resides
} SymbolNode;

SymbolNode *GetSymbolByIdent(char *pstrIdent, int iFuncIndex);

int AddSymbolNode(char *pstrIdent, int iSize, int iStackIndex, int iFuncIndex);

int GetStackIndexByIdent(char *pstrIdent, int iFuncIndex);

int GetSizeByIdent(char *pstrIdent, int iFuncIndex);

#endif //ASSEMBLER_SYMBOL_TABLE_H
