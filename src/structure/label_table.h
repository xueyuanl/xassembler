//
// Created by hsuehyuan li on 2020-03-15.
//

#ifndef ASSEMBLER_LABEL_TABLE_H
#define ASSEMBLER_LABEL_TABLE_H

#include "../lib/globals.h"
#include "../lib/linked_list.h"
#include "../lib/constants.h"
#include <string.h>

typedef struct _LabelNode       // A label table node
{
    int iIndex;                 // Index
    char pstrIdent
    [MAX_IDENT_SIZE];   // Identifier
    int iTargetIndex;           // Index of the target instruction
    int iFuncIndex;             // Function in which the label resides
} LabelNode;


LabelNode *GetLabelByIdent(char *pstrIdent, int iFuncIndex);

int AddLabelNode(char *pstrIdent, int iTargetIndex, int iFuncIndex);

#endif //ASSEMBLER_LABEL_TABLE_H
