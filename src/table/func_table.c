//
// Created by hsuehyuan li on 2020-03-15.
//

#include "func_table.h"


FuncNode *GetFuncByName(char *pstrName) {
    //If the table is empty, return a NULL pointer
    if (!g_FuncTable.iNodeCount)
        return NULL;

    // Create a pointer to traverse the list
    LinkedListNode *pCurrNode = g_FuncTable.pHead;

    // Traverse the list until the matching structure is found
    for (int iCurrNode = 0; iCurrNode < g_FuncTable.iNodeCount; ++iCurrNode) {
        // Create a pointer to the current function structure
        FuncNode *pCurrFunc = (FuncNode *) pCurrNode->pData;

        // If the names match, return the current pointer
        if (strcmp(pCurrFunc->pstrName, pstrName) == 0)
            return pCurrFunc;
        // Otherwise move to the next node
        pCurrNode = pCurrNode->pNext;
    }
    // The structure was not found, so return a NULL pointer
    return NULL;
}

int AddFunc(char *pstrName, int iEntryPoint) {
    // If a function already exists with the specified name, exit and return an invalid index
    if (GetFuncByName(pstrName))
        return -1;

    // Create a new function node
    FuncNode *pNewFunc = (FuncNode *) malloc(sizeof(FuncNode));

    // Initialize the new function
    strcpy(pNewFunc->pstrName, pstrName);
    pNewFunc->iEntryPoint = iEntryPoint;

    // Add the function to the list and get its index
    int iIndex = AddNode(&g_FuncTable, pNewFunc);

    // Set the function node's index
    pNewFunc->iIndex = iIndex;

    // Return the new function's index
    return iIndex;
}

void SetFuncInfo(char *pstrName, int iParamCount, int iLocalDataSize) {
    // Based on the function's name, find its node int the list
    FuncNode *pFunc = GetFuncByName(pstrName);

    // Set the remaining fields
    pFunc->iParamCount = iParamCount;
    pFunc->iLocalDataSize = iLocalDataSize;
}