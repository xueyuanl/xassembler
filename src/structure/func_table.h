//
// Created by hsuehyuan li on 2020-03-15.
//

#ifndef ASSEMBLER_FUNC_TABLE_H
#define ASSEMBLER_FUNC_TABLE_H

#include "../lib/constants.h"
#include "../lib/linked_list.h"
#include "../lib/globals.h"

typedef struct _FuncNode        // A function table node
{
    int iIndex;                 // Index
    char pstrName \
[MAX_IDENT_SIZE];       // name
    int iEntryPoint;            // Entry point
    int iParamCount;            // Param count
    int iLocalDataSize;         // Local data size
} FuncNode;

int AddFunc(char *pstrName, int iEntryPoint);
void SetFuncInfo(char *pstrName, int iParamCount, int iLocalDataSize);

#endif //ASSEMBLER_FUNC_TABLE_H
