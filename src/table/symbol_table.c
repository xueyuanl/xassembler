//
// Created by hsuehyuan li on 2020-03-15.
//

#include "symbol_table.h"


SymbolNode *GetSymbolByIdent(char *pstrIdent, int iFuncIndex) {
    // Local symbol node pointer

    LinkedListNode *pCurrNode = g_SymbolTable.pHead;

    // Loop through each symbol in the table to find the match

    for (int iCurrSymbolIndex = 0; iCurrSymbolIndex < g_SymbolTable.iNodeCount; ++iCurrSymbolIndex) {
        // Get the current symbol structure
        SymbolNode *pCurrSymbol = (SymbolNode *) pCurrNode->pData;
        // Return the symbol if the identifier and scope matches
        if (strcmp(pCurrSymbol->pstrIdent, pstrIdent) == 0 &&
            (pCurrSymbol->iFuncIndex == iFuncIndex || pCurrSymbol->iStackIndex >= 0))
            return pCurrSymbol;
        // Otherwise move on to the next in the list
        pCurrNode = pCurrNode->pNext;
    }

    // The symbol was not found, so return a NULL pointer
    return NULL;
}

int AddSymbolNode(char *pstrIdent, int iSize, int iStackIndex, int iFuncIndex) {
    // If a label already exists
    if (GetSymbolByIdent(pstrIdent, iFuncIndex))
        return -1;

    // Create a new symbol node
    SymbolNode *pNewSymbol = (SymbolNode *) malloc(sizeof(SymbolNode));

    // Initialize the new label
    strcpy(pNewSymbol->pstrIdent, pstrIdent);
    pNewSymbol->iSize = iSize;
    pNewSymbol->iStackIndex = iStackIndex;
    pNewSymbol->iFuncIndex = iFuncIndex;

    // Add the symbol to the list and get its index
    int iIndex = AddNode(&g_SymbolTable, pNewSymbol);

    // Set the symbol node's index
    pNewSymbol->iIndex = iIndex;

    // Return the new symbol's index
    return iIndex;
}

int GetStackIndexByIdent(char *pstrIdent, int iFuncIndex) {
    // Get the symbol's information
    SymbolNode *pSymbol = GetSymbolByIdent(pstrIdent, iFuncIndex);
    // Return ths stack index
    return pSymbol->iStackIndex;
}

int GetSizeByIdent(char *pstrIdent, int iFuncIndex) {
    // Get the symbol's information
    SymbolNode *pSymbol = GetSymbolByIdent(pstrIdent, iFuncIndex);

    // Return its size
    return pSymbol->iSize;
}