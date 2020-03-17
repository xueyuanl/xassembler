//
// Created by hsuehyuan li on 2020-03-15.
//

#ifndef ASSEMBLER_LEXER_H
#define ASSEMBLER_LEXER_H

#include <string.h>
#include "lib/string_util.h"

typedef int Token;

// ---- Token Types -----------------------------------------------------------------------

#define TOKEN_TYPE_END_OF_STREAM        0       // End of the token stream
#define TOKEN_TYPE_INVALID              1       // Invalid token

#define TOKEN_TYPE_INT                  2       // Integer
#define TOKEN_TYPE_FLOAT                3       // Float

#define TOKEN_TYPE_IDENT                4       // Identifier

#define TOKEN_TYPE_RSRVD_VAR            5       // var/var []
#define TOKEN_TYPE_RSRVD_TRUE           6       // true
#define TOKEN_TYPE_RSRVD_FALSE          7       // false
#define TOKEN_TYPE_RSRVD_IF             8       // if
#define TOKEN_TYPE_RSRVD_ELSE           9       // else
#define TOKEN_TYPE_RSRVD_BREAK          10      // break
#define TOKEN_TYPE_RSRVD_CONTINUE       11      // continue
#define TOKEN_TYPE_RSRVD_FOR            12      // for
#define TOKEN_TYPE_RSRVD_WHILE          13      // while
#define TOKEN_TYPE_RSRVD_FUNC           14      // func
#define TOKEN_TYPE_RSRVD_RETURN         15      // return
#define TOKEN_TYPE_RSRVD_HOST           16      // host

#define TOKEN_TYPE_OP                   18      // Operator

#define TOKEN_TYPE_DELIM_COMMA          19      // ,
#define TOKEN_TYPE_DELIM_OPEN_PAREN     20      // (
#define TOKEN_TYPE_DELIM_CLOSE_PAREN    21      // )
#define TOKEN_TYPE_DELIM_OPEN_BRACE     22      // [
#define TOKEN_TYPE_DELIM_CLOSE_BRACE    23      // ]
#define TOKEN_TYPE_DELIM_OPEN_CURLY_BRACE   24  // {
#define TOKEN_TYPE_DELIM_CLOSE_CURLY_BRACE  25  // }
#define TOKEN_TYPE_DELIM_SEMICOLON      26      // ;

#define TOKEN_TYPE_STRING               27      // String

void StripComments(char *pstrSourceLine);

void TrimWhitespace(char *pstrString);

#endif //ASSEMBLER_LEXER_H
