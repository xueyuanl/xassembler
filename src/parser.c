//
// Created by hsuehyuan li on 2020-03-28.
//

#include "parser.h"

char *GetCurrLexeme() {
    // Simply return the pointer rather than making a copy
    TokenNode *pTokenNode = (TokenNode *) g_pCurrTokenNode->pData;
    return pTokenNode->lexeme;
}

Token GetCurrTokenType() {
    TokenNode *pTokenNode = (TokenNode *) g_pCurrTokenNode->pData;
    return pTokenNode->iType;
}

int SkipToNextLine() {
    Token token = GetCurrTokenType();
    while(token != TOKEN_TYPE_NEWLINE){
        token = GetNextToken();
        if (token == END_OF_TOKEN_STREAM)
            return FALSE;
    }
    return TRUE;
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

    // point to the first token
    g_pCurrTokenNode = g_TokenStream.pHead;

    while (TRUE) {
        Token CurrToken = GetNextToken();
        if (CurrToken == END_OF_TOKEN_STREAM)
            break;

        switch (CurrToken) {
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
                strcpy(pstrCurrFuncName, pstrFuncName);
                iCurrFuncIndex = iFuncIndex;
                iCurrFuncParamCount = 0;
                iCurrFuncLocalDataSize = 0;

                // Read any number of line breaks until the opening brace is found
                while (GetNextToken() == TOKEN_TYPE_NEWLINE);

                // Make sure the lexeme was an opening brace
                if (GetCurrTokenType() != TOKEN_TYPE_OPEN_BRACE)
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
                strcpy(pstrIdent, GetCurrLexeme());

                // Now determine its size by finding out if it's an array or not, otherwise
                // default to 1.
                int iSize = 1;

                // Find out if an opening bracket lies ahead
                if (GetLookAheadToken() == TOKEN_TYPE_OPEN_BRACKET) {
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
                if (!iIsFuncActive)
                    ExitOnCodeError(ERROR_MSSG_GLOBAL_PARAM);

                if (strcmp(pstrCurrFuncName, MAIN_FUNC_NAME) == 0)
                    ExitOnCodeError(ERROR_MSSG_MAIN_PARAM);


                if (GetNextToken() != TOKEN_TYPE_IDENT)
                    ExitOnCodeError(ERROR_MSSG_IDENT_EXPECTED);

                iCurrFuncParamCount++;
                break;
            }

            case TOKEN_TYPE_IDENT: {
                // Make sure it's a line label
                if (GetLookAheadToken() != TOKEN_TYPE_COLON)
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

            case TOKEN_TYPE_INSTR: {
                // Make sure we aren't in the global scope, since instructions
                // can only appear in functions

                if (!iIsFuncActive)
                    ExitOnCodeError(ERROR_MSSG_GLOBAL_INSTR);

                // Increment the instruction stream size

                ++g_iInstrStreamSize;

                break;
            }
            default:
                if (GetCurrTokenType() != TOKEN_TYPE_NEWLINE)
                    ExitOnCodeError(ERROR_MSSG_INVALID_INPUT);
        }
        if(!SkipToNextLine())
            break;
    }

    // We counted the instructions, so allocate the assembled instruction stream array
    // so the next phase can begin

    g_pInstrStream = (Instr *) malloc(g_iInstrStreamSize * sizeof(Instr));

    // Initialize every operand list pointer to NULL
    int iCurrInstrIndex;
    for (iCurrInstrIndex = 0; iCurrInstrIndex < g_iInstrStreamSize; ++iCurrInstrIndex)
        g_pInstrStream[iCurrInstrIndex].pOpList = NULL;

    // Set the current instruction index to zero
    g_iCurrInstrIndex = 0;

    // ---- Perform the second pass over the source
    // Reset the lexer so we begin at the top of the source again
    // InitLexer();
    // point to the first token
    g_pCurrTokenNode = g_TokenStream.pHead;

    while (TRUE) {
        Token CurrToken = GetNextToken();
        if (CurrToken == END_OF_TOKEN_STREAM)
            break;
        switch (GetCurrTokenType()) {
            case TOKEN_TYPE_FUNC: {
                GetNextToken();
                // Use the identifier (the current lexeme) to get it's corresponding function
                // from the table

                pCurrFunc = GetFuncByName(GetCurrLexeme());

                // set the active function flag
                iIsFuncActive = TRUE;

                // Set the parameter count to zero, since we'll need to count parameters as
                // we parse Param directives
                iCurrFuncParamCount = 0;

                // Save the function's index
                iCurrFuncIndex = pCurrFunc->iIndex;

                // Read any number of line breaks until the opening brace is found
                while (GetNextToken() == TOKEN_TYPE_NEWLINE);
                break;
            }

            case TOKEN_TYPE_CLOSE_BRACE: {
                iIsFuncActive = FALSE;

                //main func
                if (strcmp(pCurrFunc->pstrName, MAIN_FUNC_NAME) == 0) {
                    g_pInstrStream[g_iCurrInstrIndex].iOpCode = INSTR_EXIT;
                    g_pInstrStream[g_iCurrInstrIndex].iOpCount = 1;

                    g_pInstrStream[g_iCurrInstrIndex].pOpList = (Operand *) malloc(sizeof(Operand));
                    g_pInstrStream[g_iCurrInstrIndex].pOpList[0].iType = OP_TYPE_INT;
                    g_pInstrStream[g_iCurrInstrIndex].pOpList[0].iIntLiteral = 0;
                } else {
                    g_pInstrStream[g_iCurrInstrIndex].iOpCode = INSTR_RET;
                    g_pInstrStream[g_iCurrInstrIndex].iOpCount = 0;
                    g_pInstrStream[g_iCurrInstrIndex].pOpList = NULL;
                }
                g_iCurrInstrIndex++;
                break;
            }

            case TOKEN_TYPE_PARAM: {
                if (GetNextToken() != TOKEN_TYPE_IDENT)
                    ExitOnCodeError(ERROR_MSSG_IDENT_EXPECTED);
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

            case TOKEN_TYPE_INSTR: {
                // Get the instruction's info using the current lexeme (the mnemonic )
                GetInstrByMnemonic(GetCurrLexeme(), &CurrInstr);

                // Write the opcode to the stream
                g_pInstrStream[g_iCurrInstrIndex].iOpCode = CurrInstr.iOpCode;

                // Write the operand count to the stream
                g_pInstrStream[g_iCurrInstrIndex].iOpCount = CurrInstr.iOpCount;

                // Allocate space to hold the operand list
                Operand *pOpList = (Operand *) malloc(CurrInstr.iOpCount * sizeof(Operand));

                int iCurrOpIndex;
                for (iCurrOpIndex = 0; iCurrOpIndex < CurrInstr.iOpCount; ++iCurrOpIndex) {
                    OpTypes CurrOpTypes = CurrInstr.OpList[iCurrOpIndex];
                    Token InitOpToken = GetNextToken();
                    switch (InitOpToken) {
                        case TOKEN_TYPE_INT: {
                            if (CurrOpTypes & OP_FLAG_TYPE_INT) {
                                pOpList[iCurrOpIndex].iType = OP_TYPE_INT;
                                pOpList[iCurrOpIndex].iIntLiteral = atoi(GetCurrLexeme());
                            } else {
                                ExitOnCodeError(ERROR_MSSG_INVALID_OP);
                            }
                            break;
                        }

                        case TOKEN_TYPE_FLOAT: {
                            if (CurrOpTypes & OP_FLAG_TYPE_FLOAT) {
                                pOpList[iCurrOpIndex].iType = OP_TYPE_FLOAT;
                                pOpList[iCurrOpIndex].fFloatLiteral = (float) atof(GetCurrLexeme());
                            } else {
                                ExitOnCodeError(ERROR_MSSG_INVALID_OP);
                            }
                            break;
                        }

                        case TOKEN_TYPE_QUOTE: {
                            if (CurrOpTypes & OP_FLAG_TYPE_STRING) {

                                GetNextToken();

                                if (GetCurrTokenType() == TOKEN_TYPE_QUOTE) {
                                    // empty string
                                    pOpList[iCurrOpIndex].iType = OP_TYPE_INT;
                                    pOpList[iCurrOpIndex].iIntLiteral = 0;
                                } else if (GetCurrTokenType() == TOKEN_TYPE_STRING) {
                                    char *pstrString = GetCurrLexeme();
                                    // Add the string to the table, or get the index of
                                    // the existing copy
                                    int iStringIndex = AddString(&g_StringTable, pstrString);
                                    pOpList[iCurrOpIndex].iType = OP_TYPE_STRING_INDEX;
                                    pOpList[iCurrOpIndex].iStringTableIndex = iStringIndex;
                                    if (GetNextToken() != TOKEN_TYPE_QUOTE)
                                        ExitOnCharExpectedError('\\');
                                } else {
                                    ExitOnCodeError(ERROR_MSSG_INVALID_STRING);
                                }

                            } else {
                                ExitOnCodeError(ERROR_MSSG_INVALID_OP);

                            }
                            break;
                        }

                        case TOKEN_TYPE_REG_RETVAL: {
                            if (CurrOpTypes & OP_FLAG_TYPE_REG) {

                                // Set a register type
                                pOpList[iCurrOpIndex].iType = OP_TYPE_REG;
                                pOpList[iCurrOpIndex].iReg = 0;
                            } else
                                ExitOnCodeError(ERROR_MSSG_INVALID_OP);
                            break;
                        }

                        case TOKEN_TYPE_IDENT: {
                            if (CurrOpTypes & OP_FLAG_TYPE_MEM_REF) {
                                char pstrIdent[MAX_IDENT_SIZE];
                                strcpy (pstrIdent, GetCurrLexeme());
                                if (!GetSymbolByIdent(pstrIdent, iCurrFuncIndex))
                                    ExitOnCodeError(ERROR_MSSG_UNDEFINED_IDENT);
                                int iBaseIndex = GetStackIndexByIdent(pstrIdent, iCurrFuncIndex);
                                if (GetLookAheadToken() != TOKEN_TYPE_OPEN_BRACKET) {

                                    if (GetSizeByIdent(pstrIdent, iCurrFuncIndex) > 1)
                                        ExitOnCodeError(ERROR_MSSG_INVALID_ARRAY_NOT_INDEXED);

                                    pOpList[iCurrOpIndex].iType = OP_TYPE_ABS_STACK_INDEX;
                                    pOpList[iCurrOpIndex].iIntLiteral = iBaseIndex;
                                } else {

                                    if (GetNextToken() != TOKEN_TYPE_OPEN_BRACKET)
                                        ExitOnCharExpectedError('[');
                                    Token IndexToken = GetNextToken();

                                    if (IndexToken == TOKEN_TYPE_INT) {
                                        int iOffsetIndex = atoi(GetCurrLexeme());

                                        pOpList[iCurrOpIndex].iType = OP_TYPE_ABS_STACK_INDEX;
                                        pOpList[iCurrOpIndex].iStackIndex = iBaseIndex + iOffsetIndex;

                                    } else if (IndexToken == TOKEN_TYPE_IDENT) {
                                        char *pstrIndexIdent = GetCurrLexeme();
                                        if (!GetSymbolByIdent(pstrIndexIdent, iCurrFuncIndex))
                                            ExitOnCodeError(ERROR_MSSG_UNDEFINED_IDENT);

                                        if (GetSizeByIdent(pstrIndexIdent, iCurrFuncIndex) > 1)
                                            ExitOnCodeError(ERROR_MSSG_INVALID_ARRAY_INDEX);
                                        int iOffsetIndex = GetStackIndexByIdent(pstrIndexIdent, iCurrFuncIndex);
                                        pOpList[iCurrOpIndex].iType = OP_TYPE_REL_STACK_INDEX;
                                        pOpList[iCurrOpIndex].iStackIndex = iBaseIndex;
                                        pOpList[iCurrOpIndex].iOffsetIndex = iOffsetIndex;
                                    } else {
                                        ExitOnCodeError(ERROR_MSSG_INVALID_ARRAY_INDEX);
                                    }

                                    if (GetNextToken() != TOKEN_TYPE_CLOSE_BRACKET)
                                        ExitOnCharExpectedError('[');
                                }
                            }

                            if (CurrOpTypes & OP_FLAG_TYPE_LINE_LABEL) {
                                char *pstrLabelIdent = GetCurrLexeme();
                                LabelNode *pLabel = GetLabelByIdent(pstrLabelIdent, iCurrFuncIndex);

                                if (!pLabel)
                                    ExitOnCodeError(ERROR_MSSG_UNDEFINED_LINE_LABEL);

                                pOpList[iCurrOpIndex].iType = OP_TYPE_INSTR_INDEX;
                                pOpList[iCurrOpIndex].iInstrIndex = pLabel->iTargetIndex;
                            }

                            if (CurrOpTypes & OP_FLAG_TYPE_FUNC_NAME) {
                                // Get the current lexeme, which is the function name
                                char *pstrFuncName = GetCurrLexeme();

                                // Use the function name to get the function's information
                                FuncNode *pFunc = GetFuncByName(pstrFuncName);

                                // Make sure the function exists
                                if (!pFunc)
                                    ExitOnCodeError(ERROR_MSSG_UNDEFINED_FUNC);

                                // Set the operand type to function index and set its data
                                // field
                                pOpList[iCurrOpIndex].iType = OP_TYPE_FUNC_INDEX;
                                pOpList[iCurrOpIndex].iFuncIndex = pFunc->iIndex;
                            }
                            if (CurrOpTypes & OP_FLAG_TYPE_HOST_API_CALL) {
                                // Get the current lexeme, which is the host API call

                                char *pstrHostAPICall = GetCurrLexeme();

                                // Add the call to the table, or get the index of the
                                // existing copy

                                int iIndex = AddString(&g_HostAPICallTable, pstrHostAPICall);

                                // Set the operand type to host API call index and set its
                                // data field

                                pOpList[iCurrOpIndex].iType = OP_TYPE_HOST_API_CALL_INDEX;
                                pOpList[iCurrOpIndex].iHostAPICallIndex = iIndex;
                            }
                            break;
                        }
                        default:
                            ExitOnCodeError(ERROR_MSSG_INVALID_OP);
                    }
                    if (iCurrOpIndex < CurrInstr.iOpCount - 1)
                        if (GetNextToken() != TOKEN_TYPE_COMMA)
                            ExitOnCharExpectedError(',');
                }
                //if ( GetNextToken () != TOKEN_TYPE_NEWLINE )
                //    ExitOnCodeError ( ERROR_MSSG_INVALID_INPUT );

                g_pInstrStream[g_iCurrInstrIndex].pOpList = pOpList;
                ++g_iCurrInstrIndex;
                break;
            }
        }
        if (!SkipToNextLine())
            break;
    }

}
