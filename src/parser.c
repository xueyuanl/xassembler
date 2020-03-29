//
// Created by hsuehyuan li on 2020-03-28.
//

#include "parser.h"

char *GetCurrLexeme() {
    // Simply return the pointer rather than making a copy
    return g_Lexer.pstrCurrLexeme;
}

void parse() {
    // ---- Initialize the script header
    g_ScriptHeader.iStackSize = 0;
    g_ScriptHeader.iIsMainFuncPresent = FALSE;

    // ---- Set some initial variables
    g_iInstrStreamSize = 0;
    g_iIsSetStackSizeFound = FALSE;
    g_ScriptHeader.iGlobalDataSize = 0;

    // Set the current function's flags and variables
    int iIsFuncActive = FALSE;
    FuncNode *pCurrFunc;
    int iCurrFuncIndex;
    char pstrCurrFuncName[MAX_IDENT_SIZE];
    int iCurrFuncParamCount = 0;
    int iCurrFuncLocalDataSize = 0;

    // Create an instruction definition structure to hold instruction information
    // when dealing with instructions.
    Instruction CurrInstr;

    // Reset the lexer
    ResetLexer();

    while (TRUE) {
        if (GetNextToken() == END_OF_TOKEN_STREAM)
            break;


        switch (g_Lexer.CurrToken) {
            case TOKEN_TYPE_SETSTACKSIZE: {
                // SetStackSize can only be found in the global scope, so make sure we
                // aren't in a function.
                if (iIsFuncActive)
                    ExitOnCodeError(ERROR_MSSG_LOCAL_SETSTACKSIZE);

                // It can only be found once, so make sure we haven't already found it
                if (g_iIsSetStackSizeFound)
                    ExitOnCodeError(ERROR_MSSG_MULTIPLE_SETSTACKSIZES);

                // Read the next lexeme, which should contain the stack size
                if (GetNextToken() != TOKEN_TYPE_INT)
                    ExitOnCodeError(ERROR_MSSG_INVALID_STACK_SIZE);

                // Convert the lexeme to an integer value from its string
                // representation and store it in the script header
                g_ScriptHeader.iStackSize = atoi(GetCurrLexeme());

                // Mark the presence of SetStackSize for future encounters
                g_iIsSetStackSizeFound = TRUE;

                break;
            }

            case TOKEN_TYPE_FUNC: {
                // First make sure we aren't in a function already, since nested functions are illegal
                if (iIsFuncActive)
                    ExitOnCodeError(ERROR_MSSG_NESTED_FUNC);

                // Read the next lexeme, which is the function name
                if (GetNextToken() != TOKEN_TYPE_IDENT)
                    ExitOnCodeError(ERROR_MSSG_IDENT_EXPECTED);
                char *pstrFuncName = GetCurrLexeme();

                // Calculate the function's entry point, which is the instruction
                // immediately following the current one, which is in turn equal to the
                // instruction stream size
                int iEntryPoint = g_iInstrStreamSize;

                // Try adding it to the function table, and print an error if it's already
                // been declared
                int iFuncIndex = AddFunc(pstrFuncName, iEntryPoint);
                if (iFuncIndex == -1)
                    ExitOnCodeError(ERROR_MSSG_FUNC_REDEFINITION);

                // Is this the _Main () function?
                if (strcmp(pstrFuncName, MAIN_FUNC_NAME) == 0) {
                    g_ScriptHeader.iIsMainFuncPresent = TRUE;
                    g_ScriptHeader.iMainFuncIndex = iFuncIndex;
                }

                // Set the function flag to true for any future encounters and
                // reinitialize function tracking variables
                iIsFuncActive = TRUE;
                strcpy (pstrCurrFuncName, pstrFuncName);
                iCurrFuncIndex = iFuncIndex;
                iCurrFuncParamCount = 0;
                iCurrFuncParamCount = 0;
                iCurrFuncLocalDataSize = 0;

                // Read any number of line breaks until the opening brace is found
                while (GetNextToken() == TOKEN_TYPE_NEWLINE);

                // Make sure the lexeme was an opening brace
                if (g_Lexer.CurrToken != TOKEN_TYPE_OPEN_BRACE)
                    ExitOnCharExpectedError('{');

                // All functions are automatically appended with Ret, so increment the
                // required size of the instruction stream
                ++g_iInstrStreamSize;

                break;
            }

            case TOKEN_TYPE_CLOSE_BRACE: {
                // This should be closing a function, so make sure we're in one
                if (!iIsFuncActive)
                    ExitOnCharExpectedError('}');
                // Set the fields we've collected
                SetFuncInfo(pstrCurrFuncName, iCurrFuncParamCount, iCurrFuncLocalDataSize);
                // Close the function
                iIsFuncActive = FALSE;
                break;
            }

            case TOKEN_TYPE_VAR: {
                // Get the variable's identifier
                if (GetNextToken() != TOKEN_TYPE_IDENT)
                    ExitOnCodeError(ERROR_MSSG_IDENT_EXPECTED);
                char pstrIdent[MAX_IDENT_SIZE];
                strcpy (pstrIdent, GetCurrLexeme());

                // Now determine its size by finding out if it's an array or not, otherwise
                // default to 1.
                int iSize = 1;

                // Find out if an opening bracket lies ahead
                if (GetLookAheadChar() == '[') {
                    // Validate and consume the opening bracket
                    if (GetNextToken() != TOKEN_TYPE_OPEN_BRACKET)
                        ExitOnCharExpectedError('[');

                    // We're parsing an array, so the next lexeme should be an integer
                    // describing the array's size
                    if (GetNextToken() != TOKEN_TYPE_INT)
                        ExitOnCodeError(ERROR_MSSG_INVALID_ARRAY_SIZE);
                    // Convert the size lexeme to an integer value
                    iSize = atoi(GetCurrLexeme());
                    // Make sure the size is valid, in that it's greater than zero
                    if (iSize <= 0)
                        ExitOnCodeError(ERROR_MSSG_INVALID_ARRAY_SIZE);
                    // Make sure the closing bracket is present as well
                    if (GetNextToken() != TOKEN_TYPE_CLOSE_BRACKET)
                        ExitOnCharExpectedError(']');
                }


                // Determine the variable's index into the stack

                // If the variable is local, then its stack index is always the local data
                // size + 2 subtracted from zero
                int iStackIndex;
                if (iIsFuncActive)
                    iStackIndex = -(iCurrFuncLocalDataSize + 2);
                    // Otherwise it's global, so it's equal to the current global data size
                else
                    iStackIndex = g_ScriptHeader.iGlobalDataSize;

                // Attempt to add the symbol to the table
                if (AddSymbolNode(pstrIdent, iSize, iStackIndex, iCurrFuncIndex) == -1)
                    ExitOnCodeError(ERROR_MSSG_IDENT_REDEFINITION);

                // Depending on the scope, increment either the local or global data size
                // by the size of the variable
                if (iIsFuncActive)
                    iCurrFuncLocalDataSize += iSize;
                else
                    g_ScriptHeader.iGlobalDataSize += iSize;
                break;
            }

            case TOKEN_TYPE_PARAM: {
                // Read the next token to get the identifier
                if (GetNextToken() != TOKEN_TYPE_IDENT)
                    ExitOnCodeError(ERROR_MSSG_IDENT_EXPECTED);

                // Read the identifier, which is the current lexeme
                char *pstrIdent = GetCurrLexeme();

                // Calculate the parameter's stack index
                int iStackIndex = -(pCurrFunc->iLocalDataSize + 2 + (iCurrFuncParamCount + 1));

                // Add the parameter to the symbol table
                if (AddSymbolNode(pstrIdent, 1, iStackIndex, iCurrFuncIndex) == -1)
                    ExitOnCodeError(ERROR_MSSG_IDENT_REDEFINITION);

                // Increment the current parameter count
                ++iCurrFuncParamCount;
                break;
            }

            case TOKEN_TYPE_IDENT: {
                // Make sure it's a line label
                if (GetLookAheadChar() != ':')
                    ExitOnCodeError(ERROR_MSSG_INVALID_INSTR);

                // Make sure we're in a function, since labels can only appear there
                if (!iIsFuncActive)
                    ExitOnCodeError(ERROR_MSSG_GLOBAL_LINE_LABEL);

                // The current lexeme is the label's identifier
                char *pstrIdent = GetCurrLexeme();

                // The target instruction is always the value of the current
                // instruction count, which is the current size - 1
                int iTargetIndex = g_iInstrStreamSize - 1;

                // Save the label's function index as well
                int iFuncIndex = iCurrFuncIndex;

                // Try adding the label to the label table, and print an error if it
                // already exists
                if (AddLabelNode(pstrIdent, iTargetIndex, iFuncIndex) == -1)
                    ExitOnCodeError(ERROR_MSSG_LINE_LABEL_REDEFINITION);
                break;
            }

        }
    }

}
