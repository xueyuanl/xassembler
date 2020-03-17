//
// Created by hsuehyuan li on 2020-03-15.
//

#include "linked_list.h"


void InitLinkedList(LinkedList *pList) {
    // Set both the head and tail pointers to null
    pList->pHead = NULL;
    pList->pHead = NULL;

    // Set the node count to zero, since the list is currently empty
    pList->iNodeCount = 0;
}

int AddNode(LinkedList *pList, void *pData) {
    // Create a new node
    LinkedListNode *pNewNode = (LinkedListNode *) malloc(sizeof(LinkedListNode));

    // Set the node's data to the specified pointer
    pNewNode->pData = pData;

    // Set the next pointer to NULL, since nothing will lie beyond it
    pNewNode->pNext = NULL;

    // If the list is currently empty, set both the head and tail pointers
    // to the new node
    if (!pList->iNodeCount) {
        // Point the head and tail of the list at the node
        pList->pHead = pNewNode;
        pList->pTail = pNewNode;
    }
        // Otherwise append it to the list and update the tail pointer
    else {
        // Alter the tail's next pinter to point to the new node
        pList->pTail->pNext = pNewNode;
        // Update the list's tail pointer
        pList->pTail = pNewNode;
    }

    // Increment the node count
    ++pList->iNodeCount;

    // Return the new size of the linked list - 1, which is the node's index
    return pList->iNodeCount - 1;
}

void FreeLinkedList(LinkedList *pList) {
    // If the list is empty, exit
    if (!pList)
        return;

    // If the list is not empty, free each node
    if (pList->iNodeCount) {
        // Create a pointer to hold each current node and the next node
        LinkedListNode *pCurrNode,
                *pNextNode;

        // Set the current node to the head of the list
        pCurrNode = pList->pHead;

        //Traverse the list
        while (TRUE) {
            // Save the pointer to the next node before freeing the current one
            pNextNode = pCurrNode->pNext;

            // Clear the current node's data
            if (pCurrNode->pData)
                free(pCurrNode->pData);

            // Clear the node itself
            // no need use if to test pCurrNode is NULL
            // if(pCurrNode)
            free(pCurrNode);

            //Move to the next node if it exists; otherwise, exit the loop
            if (pNextNode)
                pCurrNode = pNextNode;
            else
                break;
        }
    }
}