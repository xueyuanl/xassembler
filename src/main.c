#include <stdlib.h>
#include <stdio.h>
#include "lib/globals.h"
#include "parser.h"
#include "lexer.h"
#include "structure/instruction.h"

void Init() {
    // Initialize the master instruction lookup table

    InitInstrTable();

    // Initialize tables

    InitLinkedList(&g_SymbolTable);
    InitLinkedList(&g_LabelTable);
    InitLinkedList(&g_StringTable);
    InitLinkedList(&g_FuncTable);
    InitLinkedList(&g_HostAPICallTable);
}

int LoadSourceFile(char *FileName) {
    int line = 0;
    char tmparray[MAX_SOURCE_LINE_SIZE];
    int len;

    if (!(g_pSourceFile = fopen(FileName, "rb"))) {
        ExitOnError("Open Wrong!\n");
    }

    if (NULL == (g_ppstrSourceCode = (char **) malloc(sizeof(char *) * MAX_SOURCE_LIEN_NUM))) {
        ExitOnError("Malloc Wrong!\n");
    }

    while (fgets(tmparray, MAX_SOURCE_LINE_SIZE, g_pSourceFile)) {
        StripComments(tmparray);
        TrimWhiteSpace(tmparray);
        if ((len = strlen(tmparray)) <= 1)
            continue;

        if (tmparray[len - 2] != '\n') {
            tmparray[len - 1] = '\n';
            tmparray[len] = '\0';
            len++;
        }

        if (len >= MAX_SOURCE_LINE_SIZE) {
            ExitOnError("Too Long Line!\n");
        }

        g_ppstrSourceCode[line] = (char *) malloc(sizeof(char) * len);
        strcpy(g_ppstrSourceCode[line], tmparray);
        line++;
    }
    fclose(g_pSourceFile);
    return line;
}

void BuildXSE() {

    // ---- Open the output file
    FILE *pExecFile;
    if (!(pExecFile = fopen(g_pstrExecFilename, "wb")))
        ExitOnError("Could not open executable file for output");

    // ---- Write the header
    // Write the ID string (4 bytes)

    fwrite(XSE_ID_STRING, 4, 1, pExecFile);
    printf("XSE_ID: %s\n", XSE_ID_STRING);
    // Write the version (1 byte for each component, 2 total)

    char cVersionMajor = VERSION_MAJOR,
            cVersionMinor = VERSION_MINOR;
    fwrite(&cVersionMajor, 1, 1, pExecFile);
    printf("Major Version: %d\n", cVersionMajor);
    fwrite(&cVersionMinor, 1, 1, pExecFile);
    printf("Minor Version: %c\n", cVersionMinor);

    // Write the stack size (4 bytes)
    fwrite(&g_ScriptHeader.iStackSize, 4, 1, pExecFile);
    printf("Stack Size: %d\n", g_ScriptHeader.iStackSize);

    // Write the global data size (4 bytes )
    fwrite(&g_ScriptHeader.iGlobalDataSize, 4, 1, pExecFile);
    printf("Global Data Size: %d\n", g_ScriptHeader.iGlobalDataSize);

    // Write the _Main () flag (1 byte)

    char cIsMainPresent = 0;
    if (g_ScriptHeader.iIsMainFuncPresent)
        cIsMainPresent = 1;
    fwrite(&cIsMainPresent, 1, 1, pExecFile);
    printf("Main Func Present: %d\n", cIsMainPresent);

    // Write the _Main () function index (4 bytes)
    fwrite(&g_ScriptHeader.iMainFuncIndex, 4, 1, pExecFile);
    printf("Main Func Index: %d\n", g_ScriptHeader.iMainFuncIndex);

    // ---- Write the instruction stream

    // Output the instruction count (4 bytes)
    fwrite(&g_iInstrStreamSize, 4, 1, pExecFile);
    printf("Instr Stream Size: %d\n", g_iInstrStreamSize);

    // Loop through each instruction and write its data out
    for (int iCurrInstrIndex = 0; iCurrInstrIndex < g_iInstrStreamSize; ++iCurrInstrIndex) {
        // Write the opcode (2 bytes)
        short sOpcode = g_pInstrStream[iCurrInstrIndex].iOpCode;
        fwrite(&sOpcode, 2, 1, pExecFile);
        printf("OpCode: %d, ", sOpcode);

        // Write the operand count (1 byte)
        char iOpCount = g_pInstrStream[iCurrInstrIndex].iOpCount;
        fwrite(&iOpCount, 1, 1, pExecFile);
        printf("OpCount: %d, ", iOpCount);

        // Loop through the operand list and print each one out

        for (int iCurrOpIndex = 0; iCurrOpIndex < iOpCount; ++iCurrOpIndex) {
            // Make a copy of the operand pointer for convinience

            Operand CurrOp = g_pInstrStream[iCurrInstrIndex].pOpList[iCurrOpIndex];

            // Create a character for holding operand types (1 byte)
            char cOpType = CurrOp.iType;
            fwrite(&cOpType, 1, 1, pExecFile);
            printf("OpType: %d, ", cOpType);

            // Write the operand depending on its type
            switch (CurrOp.iType) {
                // Integer literal

                case OP_TYPE_INT:
                    fwrite(&CurrOp.iIntLiteral, sizeof(int), 1, pExecFile);
                    printf("Int value: %d ", CurrOp.iIntLiteral);
                    break;

                    // Floating-point literal

                case OP_TYPE_FLOAT:
                    fwrite(&CurrOp.fFloatLiteral, sizeof(float), 1, pExecFile);
                    printf("FLOAT value: %f ", CurrOp.fFloatLiteral);
                    break;

                    // String index

                case OP_TYPE_STRING_INDEX:
                    fwrite(&CurrOp.iStringTableIndex, sizeof(int), 1, pExecFile);
                    printf("Str Index: %d ", CurrOp.iStringTableIndex);
                    break;

                    // Instruction index

                case OP_TYPE_INSTR_INDEX:
                    fwrite(&CurrOp.iInstrIndex, sizeof(int), 1, pExecFile);
                    printf("Instr Index: %d ", CurrOp.iInstrIndex);
                    break;

                    // Absolute stack index

                case OP_TYPE_ABS_STACK_INDEX:
                    fwrite(&CurrOp.iStackIndex, sizeof(int), 1, pExecFile);
                    printf("Abs Stack Index: %d ", CurrOp.iStackIndex);
                    break;

                    // Relative stack index

                case OP_TYPE_REL_STACK_INDEX:
                    fwrite(&CurrOp.iStackIndex, sizeof(int), 1, pExecFile);
                    fwrite(&CurrOp.iOffsetIndex, sizeof(int), 1, pExecFile);
                    printf("rel Stack Index: %d ", CurrOp.iOffsetIndex);
                    break;

                    // Function index

                case OP_TYPE_FUNC_INDEX:
                    fwrite(&CurrOp.iFuncIndex, sizeof(int), 1, pExecFile);
                    printf("Func Index: %d ", CurrOp.iFuncIndex);
                    break;

                    // Host API call index

                case OP_TYPE_HOST_API_CALL_INDEX:
                    fwrite(&CurrOp.iHostAPICallIndex, sizeof(int), 1, pExecFile);
                    printf("Host API Call Index: %d ", CurrOp.iHostAPICallIndex);
                    break;

                    // Register

                case OP_TYPE_REG:
                    fwrite(&CurrOp.iReg, sizeof(int), 1, pExecFile);
                    printf("REG Index: %d ", CurrOp.iReg);
                    break;
            }
        }
        printf("\n");
    }

    // Create a node pointer for traversing the lists

    int iCurrNode;
    LinkedListNode *pNode;

    // ---- Write the string table

    // Write out the string count (4 bytes)
    fwrite(&g_StringTable.iNodeCount, 4, 1, pExecFile);
    printf("String count: %d\n", g_StringTable.iNodeCount);

    // Set the pointer to the head of the list

    pNode = g_StringTable.pHead;

    // Create a character for writing parameter counts

    char cParamCount;

    // Loop through each node in the list and write out its string

    for (iCurrNode = 0; iCurrNode < g_StringTable.iNodeCount; ++iCurrNode) {
        // Copy the string and calculate its length

        char *pstrCurrString = (char *) pNode->pData;
        int iCurrStringLength = strlen(pstrCurrString);

        // Write the length (4 bytes), followed by the string data (N bytes)

        fwrite(&iCurrStringLength, 4, 1, pExecFile);
        printf("Str length: %d, ", iCurrStringLength);
        fwrite(pstrCurrString, strlen(pstrCurrString), 1, pExecFile);
        printf("Str Value: %s\n", pstrCurrString);
        // Move to the next node

        pNode = pNode->pNext;
    }

    // ---- Write the function table

    // Write out the function count (4 bytes)

    fwrite(&g_FuncTable.iNodeCount, 4, 1, pExecFile);
    printf("Func count: %d\n", g_FuncTable.iNodeCount);

    // Set the pointer to the head of the list

    pNode = g_FuncTable.pHead;

    // Loop through each node in the list and write out its function info

    for (iCurrNode = 0; iCurrNode < g_FuncTable.iNodeCount; ++iCurrNode) {
        // Create a local copy of the function

        FuncNode *pFunc = (FuncNode *) pNode->pData;

        // Write the entry point (4 bytes)

        fwrite(&pFunc->iEntryPoint, sizeof(int), 1, pExecFile);
        printf("Entry Point: %d, ", pFunc->iEntryPoint);

        // Write the parameter count (1 byte)

        cParamCount = pFunc->iParamCount;
        fwrite(&cParamCount, 1, 1, pExecFile);
        printf("Param Count: %d, ", cParamCount);

        // Write the local data size (4 bytes)

        fwrite(&pFunc->iLocalDataSize, sizeof(int), 1, pExecFile);
        printf("Local Data Size: %d\n", pFunc->iLocalDataSize);

        // Move to the next node

        pNode = pNode->pNext;
    }

    // ---- Write the host API call table

    // Write out the call count (4 bytes)

    fwrite(&g_HostAPICallTable.iNodeCount, 4, 1, pExecFile);
    printf("Host API Call count: %d\n", g_HostAPICallTable.iNodeCount);

    // Set the pointer to the head of the list

    pNode = g_HostAPICallTable.pHead;

    // Loop through each node in the list and write out its string

    for (iCurrNode = 0; iCurrNode < g_HostAPICallTable.iNodeCount; ++iCurrNode) {
        // Copy the string pointer and calculate its length

        char *pstrCurrHostAPICall = (char *) pNode->pData;
        char cCurrHostAPICallLength = strlen(pstrCurrHostAPICall);

        // Write the length (1 byte), followed by the string data (N bytes)

        fwrite(&cCurrHostAPICallLength, 1, 1, pExecFile);
        printf("Host API Call Length: %d, ", cCurrHostAPICallLength);
        fwrite(pstrCurrHostAPICall, strlen(pstrCurrHostAPICall), 1, pExecFile);
        printf("Host API Call is: %s\n", pstrCurrHostAPICall);

        // Move to the next node

        pNode = pNode->pNext;
    }

    // ---- Close the output file

    fclose(pExecFile);
}

void PrintText() {
    printf("\n------------------------\n");
    int i;
    for (i = 0; i < g_iSourceFileLine; i++) {
        printf("%s", g_ppstrSourceCode[i]);
    }
    printf("------------------------\n");
}

void PrintTableInfo() {
    int i;
    LinkedListNode *tmp = g_StringTable.pHead;
    printf("String Table:\n");
    while (tmp) {
        printf("%s\n", (char *) tmp->pData);
        tmp = tmp->pNext;
    }

    tmp = g_FuncTable.pHead;
    printf("Function Table:\n");
    while (tmp) {
        printf("name:%s EP:%d paraC:%d lsize:%d\n", ((FuncNode *) tmp->pData)->pstrName,
               ((FuncNode *) tmp->pData)->iEntryPoint, ((FuncNode *) tmp->pData)->iParamCount,
               ((FuncNode *) tmp->pData)->iLocalDataSize);
        tmp = tmp->pNext;
    }

    tmp = g_SymbolTable.pHead;
    printf("Symble Table:\n");
    while (tmp) {
        printf("name:%s S:%d SI:%d FI:%d\n", ((SymbolNode *) tmp->pData)->pstrIdent, ((SymbolNode *) tmp->pData)->iSize,
               ((SymbolNode *) tmp->pData)->iStackIndex, ((SymbolNode *) tmp->pData)->iFuncIndex);
        tmp = tmp->pNext;
    }

    tmp = g_LabelTable.pHead;
    printf("Label Table:\n");
    while (tmp) {
        printf("name:%s TI:%d FI:%d\n", ((LabelNode *) tmp->pData)->pstrIdent, ((LabelNode *) tmp->pData)->iTargetIndex,
               ((LabelNode *) tmp->pData)->iFuncIndex);
        tmp = tmp->pNext;
    }

    tmp = g_HostAPICallTable.pHead;
    printf("HostAPICallTable:\n");
    while (tmp) {
        printf("%s\n", (char *) tmp->pData);
        tmp = tmp->pNext;
    }

    printf("Instruction Stream\n");
    for (i = 0; i < g_iInstrStreamSize; i++) {
        printf("OP:%d COUNT:%d\n", g_pInstrStream[i].iOpCode, g_pInstrStream[i].iOpCount);
    }
}

void ShutDown() {
    int i;
    for (i = 0; i < g_iSourceFileLine; i++) {
        free(g_ppstrSourceCode[i]);
    }
    free(g_ppstrSourceCode);

    for (i = 0; i < g_iInstrStreamSize; i++) {
        if (g_pInstrStream[i].iOpCount) {
            free(g_pInstrStream[i].pOpList);
        }
    }
    free(g_pInstrStream);

    FreeLinkedList(&g_StringTable);
    FreeLinkedList(&g_FuncTable);
    FreeLinkedList(&g_SymbolTable);
    FreeLinkedList(&g_LabelTable);
    FreeLinkedList(&g_HostAPICallTable);

}

int main(int argc, char *argv[]) {
//    /*if (argc == 1) {
////        printf("need a file!\n");
////        exit(1);
////    }
////
////    if (argc == 3) {
////        strcpy(g_pstrExecFilename, argv[2]);
////    }*/

    Init();
    g_iSourceFileLine = LoadSourceFile("/home/mystic/workspace/xassembler/src/script");
    parse();
    //PrintTableInfo();
    BuildXSE();

    ShutDown();
    return 0;
}