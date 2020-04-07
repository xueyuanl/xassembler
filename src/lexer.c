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

void TrimWhiteSpace(char *pstrString) {
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
        // fix: should be '>= 0' not '> 0'
        for (iCurrCharIndex = iStringLength - 1; iCurrCharIndex >= 0; --iCurrCharIndex) {
            if (!IsCharWhitespace(pstrString[iCurrCharIndex])) {
                pstrString[iCurrCharIndex + 1] = '\0';
                break;
            }
        }
    }
}


Token GetNextToken() {
    g_pCurrTokenNode = g_pCurrTokenNode->pNext;
    if (g_pCurrTokenNode == NULL)
        return END_OF_TOKEN_STREAM;
    TokenNode *tokenNode = (TokenNode *) g_pCurrTokenNode->pData;
    return tokenNode->iType;
}

Token _GetNextToken() {
    // ---- Lexeme Extraction

    // Move the first index (index0) past the end of the last token,
    // which is marked by the second index (index1).

    g_Lexer.iIndex0 = g_Lexer.iIndex1;

    // Make sure we aren't past the end of the current line. If a string is
    // 8 characters long, it's indexed from 0 to 7; therefore, indices 8
    // and beyond lie outside of the string and require us to move to the
    // next line. This is why I use >= for the comparison rather than >.
    // The value returned by strlen() is always one greater than the last
    // valid character index.

    if (g_Lexer.iIndex0 >= strlen(g_ppstrSourceCode[g_Lexer.iCurrSourceLine])) {
        // If so, skip to the next line but make sure we don't go past the
        // end of the file. SkipToNextLine () will return FALSE if we hit
        // the end of the file, which is the end of the token stream.
        if (!_SkipToNextLine())
            return END_OF_TOKEN_STREAM;
    }

    // If we just ended a string, tell the lexer to stop lexing
    // strings and return to the normal state
    if (g_Lexer.iCurrLexState == LEX_STATE_END_STRING)
        g_Lexer.iCurrLexState = LEX_STATE_NO_STRING;

    // Scan through the potential whitespace preceding the next lexeme, but
    // ONLY if we're not currently parsing a string lexeme (since strings
    // can contain arbitrary whitespace which must be preserved).

    if (g_Lexer.iCurrLexState != LEX_STATE_IN_STRING) {
        // Scan through the whitespace and check for the end of the line
        while (TRUE) {
            // If the current character is not whitespace, exit the loop
            // because the lexeme is starting.
            if (!IsCharWhitespace(g_ppstrSourceCode[g_Lexer.iCurrSourceLine][g_Lexer.iIndex0]))
                break;
            // It is whitespace, however, so move to the next character and
            // continue scanning
            ++g_Lexer.iIndex0;
        }
    }

    // Bring the second index (Index1) to the lexeme's starting character,
    // which is marked by the first index (Index0)
    g_Lexer.iIndex1 = g_Lexer.iIndex0;

    // Scan through the lexeme until a delimiter is hit, incrementing
    // Index1 each time
    while (TRUE) {
        // Are we currently scanning through a string?
        if (g_Lexer.iCurrLexState == LEX_STATE_IN_STRING) {
            // If we're at the end of the line, return an invalid token
            // since the string has no ending double-quote on the line
            if (g_Lexer.iIndex1 >= strlen(g_ppstrSourceCode[g_Lexer.iCurrSourceLine])) {
                g_Lexer.CurrToken = TOKEN_TYPE_INVALID;
                return g_Lexer.CurrToken;
            }

            // If the current character is a backslash, move ahead two
            // characters to skip the escape sequence and jump to the next
            // iteration of the loop
            if (g_ppstrSourceCode[g_Lexer.iCurrSourceLine][g_Lexer.iIndex1] == '\\') {
                g_Lexer.iIndex1 += 2;
                continue;
            }

            // If the current character isn't a double-quote, move to the
            // next, otherwise exit the loop, because the string has ended.
            if (g_ppstrSourceCode[g_Lexer.iCurrSourceLine][g_Lexer.iIndex1] == '"')
                break;
            ++g_Lexer.iIndex1;
        }
            // We are not currently scanning through a string
        else {
            // If we're at the end of the line, the lexeme has ended so
            // exit the loop
            if (g_Lexer.iIndex1 >= strlen(
                    g_ppstrSourceCode[g_Lexer.iCurrSourceLine]))
                break;
            // If the current character isn't a delimiter, move to the
            // next, otherwise exit the loop
            if (IsCharDelimiter(g_ppstrSourceCode[g_Lexer.iCurrSourceLine][g_Lexer.iIndex1]))
                break;
            ++g_Lexer.iIndex1;
        }
    }

    // Single-character lexemes will appear to be zero characters at this
    // point (since Index1 will equal Index0), so move Index1 over by one
    // to give it some noticeable width
    if (g_Lexer.iIndex1 - g_Lexer.iIndex0 == 0)
        ++g_Lexer.iIndex1;

    // The lexeme has been isolated and lies between Index0 and Index1
    // (inclusive), so make a local copy for the lexer
    unsigned int iCurrDestIndex = 0;
    for (unsigned int iCurrSourceIndex = g_Lexer.iIndex0; iCurrSourceIndex < g_Lexer.iIndex1; ++iCurrSourceIndex) {
        // If we're parsing a string, check for escape sequences and just
        // copy the character after the backslash
        if (g_Lexer.iCurrLexState == LEX_STATE_IN_STRING)
            if (g_ppstrSourceCode[g_Lexer.iCurrSourceLine][iCurrSourceIndex] == '\\')
                ++iCurrSourceIndex;
        // Copy the character from the source line to the lexeme
        g_Lexer.pstrCurrLexeme[iCurrDestIndex] = g_ppstrSourceCode[g_Lexer.iCurrSourceLine][iCurrSourceIndex];
        // Advance the destination index
        ++iCurrDestIndex;
    }

    // Set the null terminator
    g_Lexer.pstrCurrLexeme[iCurrDestIndex] = '\0';

    // Convert it to uppercase if it's not a string
    if (g_Lexer.iCurrLexState != LEX_STATE_IN_STRING)
        strupr(g_Lexer.pstrCurrLexeme);

    // ---- Token Identification
    // Let's find out what sort of token our new lexeme is
    // We'll set the type to invalid now just in case the lexer doesn't
    // match any token types
    g_Lexer.CurrToken = TOKEN_TYPE_INVALID;

    // The first case is the easiest-- if the string lexeme state is
    // active, we know we're dealing with a string token. However, if the
    // string is the double-quote sign, it means we've read an empty string
    // and should return a double-quote instead
    if (strlen(g_Lexer.pstrCurrLexeme) > 1 || g_Lexer.pstrCurrLexeme[0] != '"') {
        if (g_Lexer.iCurrLexState == LEX_STATE_IN_STRING) {
            g_Lexer.CurrToken = TOKEN_TYPE_STRING;
            return TOKEN_TYPE_STRING;
        }
    }

    // Now let's check for the single-character tokens
    if (strlen(g_Lexer.pstrCurrLexeme) == 1) {
        switch (g_Lexer.pstrCurrLexeme[0]) {
            // Double-Quote
            case '"':
                // If a quote is read, advance the lexing state so that
                // strings are lexed properly
                switch (g_Lexer.iCurrLexState) {
                    // If we're not lexing strings, tell the lexer we're
                    // now in a string
                    case LEX_STATE_NO_STRING:
                        g_Lexer.iCurrLexState = LEX_STATE_IN_STRING;
                        break;
                        // If we're in a string, tell the lexer we just ended a
                        // string
                    case LEX_STATE_IN_STRING:
                        g_Lexer.iCurrLexState = LEX_STATE_END_STRING;
                        break;
                }
                g_Lexer.CurrToken = TOKEN_TYPE_QUOTE;
                break;
                // Comma
            case ',':
                g_Lexer.CurrToken = TOKEN_TYPE_COMMA;
                break;
                // Colon
            case ':':
                g_Lexer.CurrToken = TOKEN_TYPE_COLON;
                break;
                // Opening Bracket
            case '[':
                g_Lexer.CurrToken = TOKEN_TYPE_OPEN_BRACKET;
                break;
                // Closing Bracket
            case ']':
                g_Lexer.CurrToken = TOKEN_TYPE_CLOSE_BRACKET;
                break;
                // Opening Brace
            case '{':
                g_Lexer.CurrToken = TOKEN_TYPE_OPEN_BRACE;
                break;
                // Closing Brace
            case '}':
                g_Lexer.CurrToken = TOKEN_TYPE_CLOSE_BRACE;
                break;
                // Newline
            case '\n':
                g_Lexer.CurrToken = TOKEN_TYPE_NEWLINE;
                break;
        }
    }
    // Now let's check for the multi-character tokens
    // Is it an integer?
    if (IsStringInteger(g_Lexer.pstrCurrLexeme))
        g_Lexer.CurrToken = TOKEN_TYPE_INT;
    // Is it a float?
    if (IsStringFloat(g_Lexer.pstrCurrLexeme))
        g_Lexer.CurrToken = TOKEN_TYPE_FLOAT;
    // Is it an identifier (which may also be a line label or instruction)?
    if (IsStringIdent(g_Lexer.pstrCurrLexeme))
        g_Lexer.CurrToken = TOKEN_TYPE_IDENT;
    // Check for directives or _RetVal
    // Is it SetStackSize?
    if (strcmp(g_Lexer.pstrCurrLexeme, "SETSTACKSIZE") == 0)
        g_Lexer.CurrToken = TOKEN_TYPE_SETSTACKSIZE;
    // Is it Var/Var []?
    if (strcmp(g_Lexer.pstrCurrLexeme, "VAR") == 0)
        g_Lexer.CurrToken = TOKEN_TYPE_VAR;
    // Is it Func?
    if (strcmp(g_Lexer.pstrCurrLexeme, "FUNC") == 0)
        g_Lexer.CurrToken = TOKEN_TYPE_FUNC;
    // Is it Param?
    if (strcmp(g_Lexer.pstrCurrLexeme, "PARAM") == 0)
        g_Lexer.CurrToken = TOKEN_TYPE_PARAM;
    // Is it RetVal?
    if (strcmp(g_Lexer.pstrCurrLexeme, "_RETVAL") == 0)
        g_Lexer.CurrToken = TOKEN_TYPE_REG_RETVAL;
    // Is it an instruction?
    Instruction Instr;
    if (GetInstrByMnemonic(g_Lexer.pstrCurrLexeme, &Instr))
        g_Lexer.CurrToken = TOKEN_TYPE_INSTR;
    return g_Lexer.CurrToken;
}

int _SkipToNextLine() {
    // Increment the current line
    ++g_Lexer.iCurrSourceLine;

    // Return FALSE if we've gone past the end of the source code
    if (g_Lexer.iCurrSourceLine >= g_iSourceFileLine)
        return FALSE;

    // Set both indices to point to the start of the string
    g_Lexer.iIndex0 = 0;
    g_Lexer.iIndex1 = 0;

    // Turn off string lexeme mode, since strings can't span multiple lines
    g_Lexer.iCurrLexState = LEX_STATE_NO_STRING;

    // Return TRUE to indicate success
    return TRUE;

}

void InitLexer() {
    // Set the current line to the start of the file
    g_Lexer.iCurrSourceLine = 0;
    // Set both indices to point to the start of the string
    g_Lexer.iIndex0 = 0;
    g_Lexer.iIndex1 = 0;
    // Set the token type to invalid, since a token hasn't been read yet
    g_Lexer.CurrToken = TOKEN_TYPE_INVALID;
    // Set the lexing state to no strings
    g_Lexer.iCurrLexState = LEX_STATE_NO_STRING;
}

Token GetLookAheadToken() {
    TokenNode *tokenNode = (TokenNode *) g_pCurrTokenNode->pNext->pData;
    return tokenNode->iType;
}

void _lexer() {
    InitLinkedList(&g_TokenStream);
    // grantee first invoke GetNextToken() to get the genuine first Token.
    AddToken(HEAD_OF_TOKEN_STREAM, "Token Placeholder");

    InitLexer();

    while (_GetNextToken() != END_OF_TOKEN_STREAM) {
        AddToken(g_Lexer.CurrToken, g_Lexer.pstrCurrLexeme);
    }
}