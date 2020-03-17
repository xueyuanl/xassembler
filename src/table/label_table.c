//
// Created by hsuehyuan li on 2020-03-15.
//

#include "label_table.h"


LabelNode *GetLabelByIdent(char *pstrIdent, int iFuncIndex) {
    // If the table is empty, return a NULL pointer
    if (!g_LabelTable.iNodeCount)
        return NULL;
    // Local symbol node pointer
    LinkedListNode *pCurrNode = g_LabelTable.pHead;

    // Loop through each label in the table to find the match
    for (int iCurrNode = 0; iCurrNode < g_SymbolTable.iNodeCount; ++iCurrNode) {
        // Get the current symbol structure
        LabelNode *pCurrLabel = (LabelNode *) pCurrNode->pData;
        // If the names and scopes match, return the current pointer
        if (strcmp(pCurrLabel->pstrIdent, pstrIdent) == 0 && pCurrLabel->iFuncIndex == iFuncIndex)
            return pCurrLabel;
        // Otherwise move on to the next in the list
        pCurrNode = pCurrNode->pNext;
    }

    // The structure was not found, so return a NULL pointer
    return NULL;
}

int AddLabelNode(char *pstrIdent, int iTargetIndex, int iFuncIndex) {
    // If a label already exists
    if (GetLabelByIdent(pstrIdent, iFuncIndex))
        return -1;

    // Create a new label node
    LabelNode *pNewLabel = (LabelNode *) malloc(sizeof(LabelNode));

    // Initialize the new label
    strcpy(pNewLabel->pstrIdent, pstrIdent);
    pNewLabel->iTargetIndex = iTargetIndex;
    pNewLabel->iFuncIndex = iFuncIndex;

    // Add the symbol to the list and get its index
    int iIndex = AddNode(&g_LabelTable, pNewLabel);

    // Set the label node's index
    pNewLabel->iIndex = iIndex;

    // Return the new label's index
    return iIndex;
}