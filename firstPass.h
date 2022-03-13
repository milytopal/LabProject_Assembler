
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
    eAdrresMethod addressingMethod;
    int value;
    bool isLabel;
    char labelName[LABEL_LEN];
    bool isError;
} Argument;

Label LabelConstructor(char* name, int val, eDataType dType, eLocalizaion localize);

bool firstPass(const char* fileName, bool firstPass);
bool labelCheck(char*, char*, int);
bool isRegName(char *label);
int getRegNum(char *label);

eCommands getOpcode(char*);
int getFunct(int, char*);
Argument getArgument(char*, char*, int); 

#endif
