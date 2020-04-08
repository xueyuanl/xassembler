//
// Created by hsuehyuan li on 2020-03-25.
//

#ifndef ASSEMBLER_ERROR_H
#define ASSEMBLER_ERROR_H

#include <stdlib.h>
#include <stdio.h>
#include "globals.h"
#include "../structure/token_stream.h"

void ExitOnError(char *pstrErrorMssg);
void ExitOnCodeError(char *pstrErrorMssg);
void ExitOnCharExpectedError(char cChar);

#endif //ASSEMBLER_ERROR_H
