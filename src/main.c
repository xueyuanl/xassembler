#include <stdlib.h>
#include <stdio.h>
#include <python2.7/Python.h>
#include "lib/string_util.h"
#include "lexer.h"

PyObject *PrintString(PyObject *pSelf, PyObject *pParams) {
    char *pstrString;

    if (!PyArg_ParseTuple(pParams, "s", &pstrString)) {
        printf("Unable to parse parameter tuple.\n");
        exit(0);
    }

    printf("%s\n", pstrString);
    return PyInt_FromLong(1);
}

int main() {
    char a[] = "   This is a string.   ";
    TrimWhitespace(a);
}

int main_py(int argc, const char *argv[]) {
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("import os");
    PyRun_SimpleString("sys.path.append(os.environ[\'HOME\'] + \"/mnt/pi/assembler/src\")");

    PyObject *pName = PyString_FromString("pmodule");
    PyObject *pModule = PyImport_Import(pName);

    if (!pModule) {
        PyErr_Print();
        printf("Could not open script.\n");
    }


    PyObject *pDict = PyModule_GetDict(pModule);
    PyObject *pFunc;
    PyObject *pParams;

    // C call python method
    pFunc = PyDict_GetItemString(pDict, "get_max");

    pParams = PyTuple_New(2);
    PyObject *pCurrParam;
    pCurrParam = PyInt_FromLong(16);
    PyTuple_SetItem(pParams, 0, pCurrParam);
    pCurrParam = PyInt_FromLong(32);
    PyTuple_SetItem(pParams, 1, pCurrParam);

    PyObject *pMax = PyObject_CallObject(pFunc, pParams);
    int iMax = PyInt_AsLong(pMax);

    // python call C method
    if (!PyImport_AddModule("HostAPI")) {
        printf("Host API module could not be created.");
    }

    PyMethodDef HostAPIFuncs[] = {
            {"PrintString", PrintString, METH_VARARGS, NULL},
            {NULL, NULL,                 NULL,         NULL}
    };

    if (!Py_InitModule("HostAPI", HostAPIFuncs)) {
        printf("Host API module could not be initialized.");
    }
    pFunc = PyDict_GetItemString(pDict, "call_PrintString");
    PyObject *pBack = PyObject_CallObject(pFunc, NULL);

    Py_Finalize();
}



