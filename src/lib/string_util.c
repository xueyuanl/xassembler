//
// Created by hsuehyuan li on 2020-03-15.
//

#include "string_util.h"

// Determines if a character is a numeric digit
int IsCharNumeric(char c) {
    // Return true if the character is between 0 and 9 inclusive.
    if (c >= '0' && c <= '9')
        return TRUE;
    else return FALSE;
}

// Determines if a character is whitespace
int IsCharWhitespace(char c) {
    // Return true if the character is a space or tab.
    if (c == ' ' || c == '\t')
        return TRUE;
    else
        return FALSE;
}

// Determines if a character could be part of a valid identifier
int IsCharIdent(char c) {
    // Return true if the character is between 0 or 9 inclusive or is
    // an uppercase or lowercase letter
    if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c >= '_')
        return TRUE;
    else
        return FALSE;
}

// Determines if a character is part of a delimiter
int IsCharDelimiter(char c) {
    // Return true if the character is a delimiter
    if (c == ':' || c == ',' || c == '"' || c == '[' || c == ']' || c == '{' || c == '}' || IsCharWhitespace(c))
        return TRUE;
    else
        return FALSE;
}

int IsStringInteger(char *pstrString) {
    if (!pstrString)
        return FALSE;
    unsigned length = strlen(pstrString);
    if (length == 0)
        return FALSE;

    unsigned int iCurrCharIndex;

    for (iCurrCharIndex = 0; iCurrCharIndex < length; ++iCurrCharIndex) {
        if (!IsCharNumeric(pstrString[iCurrCharIndex]) && pstrString[iCurrCharIndex] != '-')
            return FALSE;

        if (iCurrCharIndex > 0 && pstrString[iCurrCharIndex] == '-')
            return FALSE;
    }
    return TRUE;
}

int IsStringFloat(char *pstrString) {
    if (!pstrString)
        return FALSE;
    unsigned length = strlen(pstrString);
    if (length == 0)
        return FALSE;

    unsigned int iCurrCharIndex;

    for (iCurrCharIndex = 0; iCurrCharIndex < length; ++iCurrCharIndex) {
        if (!IsCharNumeric(pstrString[iCurrCharIndex]) &&
            pstrString[iCurrCharIndex] != '-' &&
            pstrString[iCurrCharIndex] != '.')
            return FALSE;
        if (iCurrCharIndex > 0 && pstrString[iCurrCharIndex] == '-')
            return FALSE;
    }

    int iRadixPointFound = FALSE;

    for (iCurrCharIndex = 0; iCurrCharIndex < length; ++iCurrCharIndex) {
        if (pstrString[iCurrCharIndex] == '.')
            if (iRadixPointFound)
                return FALSE;
            else
                iRadixPointFound = TRUE;
    }

    if (iRadixPointFound)
        return TRUE;
    else
        return FALSE;

}

int IsStringWhitespace(char *pstrString) {
    if (!pstrString)
        return FALSE;
    unsigned length = strlen(pstrString);
    if (length == 0)
        return FALSE;
    unsigned int iCurrCharIndex;
    for (iCurrCharIndex = 0; iCurrCharIndex < length; ++iCurrCharIndex) {
        if (!IsCharWhitespace(pstrString[iCurrCharIndex]))
            return FALSE;
    }
    return TRUE;
}

int IsStringIdent(char *pstrString) {
    if (!pstrString)
        return FALSE;
    unsigned length = strlen(pstrString);
    if (length == 0)
        return FALSE;

    if (pstrString[0] >= '0' && pstrString[0] <= '9')
        return FALSE;
    unsigned int iCurrCharIndex;
    for (iCurrCharIndex = 0; iCurrCharIndex < length; ++iCurrCharIndex) {
        if (!IsCharIdent(pstrString[iCurrCharIndex]))
            return FALSE;
    }
    return TRUE;
}

void strupr(char *pstrString) {
    while (*pstrString) {
        if (*pstrString >= 'a' && *pstrString <= 'z') {
            *pstrString = *pstrString - 32;
        }
        pstrString++;
    }
}