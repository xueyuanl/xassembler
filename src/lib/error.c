//
// Created by hsuehyuan li on 2020-03-25.
//

#include "error.h"

void ExitOnError(char *pstrErrorMssg) {
    // Print the message
    printf("Fatal Error: %s.\n", pstrErrorMssg);

    // Exit the program
    exit(1);
}

void ExitOnCodeError(char *pstrErrorMssg) {
    // Print the message
    printf("Error: %s.\n\n", pstrErrorMssg);
    printf("Line %d\n", g_Lexer.iCurrSourceLine);

    // Reduce all of the source line's spaces to tabs so it takes less
    // space and so the caret lines up with the current token properly
    char pstrSourceLine[MAX_SOURCE_LINE_SIZE];
    strcpy (pstrSourceLine, g_ppstrSourceCode[g_Lexer.iCurrSourceLine]);

    // Loop through each character and replace tabs with spaces
    for (unsigned int iCurrCharIndex = 0; iCurrCharIndex < strlen(pstrSourceLine); ++iCurrCharIndex)
        if (pstrSourceLine[iCurrCharIndex] == '\t')
            pstrSourceLine[iCurrCharIndex] = ' ';

    // Print the offending source line
    printf("%s", pstrSourceLine);

    // Print a caret at the start of the (presumably) offending lexeme
    for (unsigned int iCurrSpace = 0; iCurrSpace < g_Lexer.iIndex0; ++iCurrSpace)
        printf(" ");
    printf("^\n");

    // Print message indicating that the script could not be assembled
    printf("Could not assemble %s.\n", g_pstrExecFilename);

    // Exit the program
    exit(1);
}

void ExitOnCharExpectedError(char cChar) {
    // Create an error message based on the character
    char *pstrErrorMssg = (char *) malloc(strlen("' ' expected"));
    sprintf (pstrErrorMssg, "'%c' expected", cChar);

    // Exit on the code error
    ExitOnCodeError(pstrErrorMssg);
}
