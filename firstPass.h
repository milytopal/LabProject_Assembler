
#ifndef FIRST_PASS_H
#define FIRST_PASS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "dataStructs.h"
#include "extrnVariables.h"
#include "dataLinkedList.h"
#include "wordLinkedList.h"


typedef struct Argument {
    eAddrresMethod addressingMethod;
    int value;
    bool isLabel;
    char labelName[LABEL_LEN];
    bool isError;
} Argument;

bool firstPass(const char* fileName, int *ICF, int *DCF);
bool labelCheck(char*, char*, int);
bool isRegName(const char *label);
int getRegNum(const char *label);

eCommands getOpcode(char*);
int getFunct(int, char*);
bool fillOutArguments(const char*, char*,unsigned int ,int );

#endif
