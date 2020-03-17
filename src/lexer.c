//
// Created by hsuehyuan li on 2020-03-15.
//

#include "lexer.h"


void StripComments(char *pstrSourceLine) {
    unsigned int iCurrCharIndex;
    int iInString;

    // Scan through the source line and terminate the string at the first semicolon
    iInString = 0;

    for (iCurrCharIndex = 0; iCurrCharIndex < strlen(pstrSourceLine) - 1; ++iCurrCharIndex) {
        // Look out for strings; they can contain semicolons too
        if (pstrSourceLine[iCurrCharIndex] == '"') {
            if (iInString)
                iInString = 0;
            else iInString = 1;
        }

        // If a non-string semicolon is found, terminate the string at its position
        if (pstrSourceLine[iCurrCharIndex] == ';') {
            if (!iInString) {
                pstrSourceLine[iCurrCharIndex] = '\n';
                pstrSourceLine[iCurrCharIndex + 1] = '\0';
                break;
            }
        }
    }

}

void TrimWhitespace(char *pstrString) {
    unsigned int iStringLength = strlen(pstrString);
    unsigned int iPadLength;
    unsigned int iCurrCharIndex;

    if (iStringLength > 1) {
        // First determine whitespace quantity on the left
        for (iCurrCharIndex = 0; iCurrCharIndex < iStringLength; ++iCurrCharIndex) {
            if (!IsCharWhitespace(pstrString[iCurrCharIndex]))
                break;
        }

        // Slide string to the left to overwrite whitespace
        iPadLength = iCurrCharIndex;
        if (iPadLength) {
            for (iCurrCharIndex = iPadLength; iCurrCharIndex < iStringLength; ++iCurrCharIndex) {
                pstrString[iCurrCharIndex - iPadLength] = pstrString[iCurrCharIndex];
            }

            for (iCurrCharIndex = iStringLength - iPadLength; iCurrCharIndex < iStringLength; ++iCurrCharIndex)
                pstrString[iCurrCharIndex] = ' ';
        }

        // Terminate string at the start of right hand whitespace
        for (iCurrCharIndex = iStringLength - 1; iCurrCharIndex > 0; --iCurrCharIndex) {
            if (!IsCharWhitespace(pstrString[iCurrCharIndex])) {
                pstrString[iCurrCharIndex + 1] = '\0';
                break;
            }
        }
    }
}