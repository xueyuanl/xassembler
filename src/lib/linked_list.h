//
// Created by hsuehyuan li on 2020-03-15.
//

#ifndef PLAYGROUND_LINKEDLIST_H
#define PLAYGROUND_LINKEDLIST_H

#include <stdlib.h>

#define TRUE 1
#define FALSE 0

typedef struct _LinkedListNode  // A linked list node
{
    void *pData;                // Pointer to the node's data
    struct _LinkedListNode
            *pNext;             // Pointer to the next node in the list
} LinkedListNode;

typedef struct _LinkedList      // A linked list
{
    LinkedListNode *pHead,      // Pointer to head node
            *pTail;             // Pointer to tail node
    int iNodeCount;             // The number of nodes int the list
} LinkedList;

void InitLinkedList(LinkedList *pList);

int AddNode(LinkedList *pList, void *pData);

void FreeLinkedList(LinkedList *pList);

#endif //ASSEMBLER_LINKEDLIST_H
