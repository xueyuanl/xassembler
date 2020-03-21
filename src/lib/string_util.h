//
// Created by hsuehyuan li on 2020-03-15.
//

#ifndef ASSEMBLER_STRING_UTIL_H
#define ASSEMBLER_STRING_UTIL_H


#include <string.h>

#define TRUE 1
#define FALSE 0

int IsCharWhitespace(char c);

int IsCharDelimiter(char c);

int IsStringInteger(char *pstrString);

int IsStringFloat(char *pstrString);

int IsStringWhitespace(char *pstrString);

int IsStringIdent(char *pstrString);

void strupr(char *pstrString);

#endif //ASSEMBLER_STRING_UTIL_H
