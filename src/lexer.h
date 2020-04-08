//
// Created by hsuehyuan li on 2020-03-15.
//

#ifndef ASSEMBLER_LEXER_H
#define ASSEMBLER_LEXER_H

#include <string.h>
#include "lib/string_util.h"
#include "lib/linked_list.h"
#include "lib/globals.h"
#include "structure/instruction.h"
#include "structure/token_stream.h"



// ---- Lexer States ----------------------------------------------------------------------

#define LEX_STATE_UNKNOWN               0       // Unknown lexeme type

#define LEX_STATE_START                 1       // Start state

#define LEX_STATE_INT                   2       // Integer
#define LEX_STATE_FLOAT                 3       // Float

#define LEX_STATE_IDENT                 4       // Identifier


#define LEX_STATE_OP                    5       // Operator
#define LEX_STATE_DELIM                 6       // Delimiter

#define LEX_STATE_STRING                7       // String value
#define LEX_STATE_STRING_ESCAPE         8       // Escape sequence
#define LEX_STATE_STRING_CLOSE_QUOTE    9       // String closing quote

#define LEX_STATE_END_STRING            10
#define LEX_STATE_NO_STRING             11
#define LEX_STATE_IN_STRING             12

// ---- Token Types -----------------------------------------------------------------------
#define TOKEN_TYPE_INT                  0       // Integer
#define TOKEN_TYPE_FLOAT                1       // Float
#define TOKEN_TYPE_STRING               2       // String
#define TOKEN_TYPE_QUOTE                3       // "
#define TOKEN_TYPE_IDENT                4       // Identifier
#define TOKEN_TYPE_COLON                5       // :
#define TOKEN_TYPE_OPEN_BRACKET         6       // [
#define TOKEN_TYPE_CLOSE_BRACKET        7       // ]
#define TOKEN_TYPE_COMMA                8       // ,
#define TOKEN_TYPE_OPEN_BRACE           9       // {
#define TOKEN_TYPE_CLOSE_BRACE          10      // }
#define TOKEN_TYPE_NEWLINE              11      // \n
#define TOKEN_TYPE_INSTR                12      // An instruction, MOV
#define TOKEN_TYPE_SETSTACKSIZE         13      // The setStackSize directive
#define TOKEN_TYPE_VAR                  14      // Var
#define TOKEN_TYPE_FUNC                 15      // Func
#define TOKEN_TYPE_PARAM                16      // Param
#define TOKEN_TYPE_REG_RETVAL           17      // the _RetVal register
#define TOKEN_TYPE_INVALID              18      // Error code for invalid tokens
#define END_OF_TOKEN_STREAM             19      // End of the token stream
#define HEAD_OF_TOKEN_STREAM            20      // Placeholder of the first token stream

typedef struct _Lexer               // The lexer's state
{
    unsigned int iIndex0;
    unsigned int iIndex1;
    int iCurrSourceLine;
    int iCurrLexState;              // in string or not
    Token CurrToken;
    char pstrCurrLexeme[MAX_LEXEME_SIZE];

} Lexer;

extern Lexer lexer;

void StripComments(char *pstrSourceLine);

void TrimWhiteSpace(char *pstrString);

Token GetNextToken();

void InitLexer();

Token GetLookAheadToken();

int _SkipToNextLine();

void _lexer();

#endif //ASSEMBLER_LEXER_H
