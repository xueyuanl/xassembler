//
// Created by pat on 4/6/20.
//

#ifndef ASSEMBLER_TOKEN_STREAM_H
#define ASSEMBLER_TOKEN_STREAM_H

#include "../lib/constants.h"
#include "../lib/globals.h"
typedef struct _TokenNode        // A Token Stream node
{
    int iIndex;
    int iType;                  // Token Type
    char lexeme[MAX_LEXEME_SIZE];
} TokenNode;

int AddToken(int type, char *pstrLexeme);

#endif //ASSEMBLER_TOKEN_STREAM_H
