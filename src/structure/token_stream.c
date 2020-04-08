//
// Created by pat on 4/6/20.
//
#include "token_stream.h"

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

char *GetCurrLexeme() {
    // Simply return the pointer rather than making a copy
    TokenNode *pTokenNode = (TokenNode *) g_pCurrTokenNode->pData;
    return pTokenNode->lexeme;
}

Token GetCurrTokenType() {
    TokenNode *pTokenNode = (TokenNode *) g_pCurrTokenNode->pData;
    return pTokenNode->iType;
}

Token GetCurrTokenIndex() {
    TokenNode *pTokenNode = (TokenNode *) g_pCurrTokenNode->pData;
    return pTokenNode->iIndex;
}