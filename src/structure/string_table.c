//
// Created by hsuehyuan li on 2020-03-17.
//

#include "string_table.h"


int AddString(LinkedList *pList, char *pstrString) {
    // ----- First check to see if the string is already in the list

    // Create a node pointer to traverse the list
    LinkedListNode *pNode = pList->pHead;

    // Loop through each node in the list
    for (int iCurrNode = 0; iCurrNode < pList->iNodeCount; ++iCurrNode) {
        // if the current node's string equals the specified string, retur its index
        if (strcmp((char *) pNode->pData, pstrString) == 0)
            return iCurrNode;
        // other wise move along to the next node
        pNode = pNode->pNext;
    }

    // Create space on the heap for the specified string
    char *pstrStringNode = (char *) malloc(strlen(pstrString) + 1);
    strcpy(pstrStringNode, pstrString);

    // Add the string to the list and return its index
    return AddNode(pList, pstrStringNode);
    //return 1;
}