//
// Created by pat on 4/6/20.
//
#include <stdlib.h>
#include <string.h>
#include "token_stream.h"
#include "../lib/linked_list.h"

int AddToken(int type, char *pstrLexeme) {
    // Create a new token node
    TokenNode *pNewToken = (TokenNode *) malloc(sizeof(TokenNode));

    // Initialize the new token
    strcpy(pNewToken->lexeme, pstrLexeme);
    pNewToken->iType = type;

    // Add the token node to the list and get its index
    int iIndex = AddNode(&g_TokenStream, pNewToken);

    // Set the token node's index
    pNewToken->iIndex = iIndex;

    // Return the new token's index
    return iIndex;
}