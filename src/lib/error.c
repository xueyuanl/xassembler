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
    printf("At Token stream index: %d, Token type: %d, token name: %s.\n", GetCurrTokenIndex(), GetCurrTokenType(),
           GetCurrLexeme());

    // Print message indicating that the script could not be assembled
    printf("Could not assemble %s.\n", g_pstrExecFilename);

    // Exit the program
    exit(1);
}

void ExitOnCharExpectedError(char cChar) {
    // Create an error message based on the character
    char *pstrErrorMssg = (char *) malloc(strlen("' ' expected"));
    sprintf(pstrErrorMssg, "'%c' expected", cChar);

    // Exit on the code error
    ExitOnCodeError(pstrErrorMssg);
}
