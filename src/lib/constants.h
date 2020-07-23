//
// Created by hsuehyuan li on 2020-03-15.
//

#ifndef ASSEMBLER_CONSTANTS_H
#define ASSEMBLER_CONSTANTS_H

#define TRUE                        1
#define FALSE                       0

#define MAX_IDENT_SIZE              4096

#define MAX_INSTR_MNEMONIC_SIZE     16          // Maximum size of an instruction mnemonic's string
#define MAX_INSTR_LOOKUP_COUNT      256         // The maximum number of instructions the lookup table will hold

#define MAX_LEXEME_SIZE             4096
#define MAX_SOURCE_LINE_SIZE        1024
#define MAX_SOURCE_LIEN_NUM         1024

#define MAIN_FUNC_NAME              "_MAIN"     // the function name is processed to upper case

#define XSE_ID_STRING               "XSE0"      // Written to the file to state it's
// validity
#define VERSION_MAJOR               0           // Major version number
#define VERSION_MINOR               4           // Minor version number
#endif //ASSEMBLER_CONSTANTS_H
