
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

Label* LabelConstructor(char* name, int val, eDataType dType, eLocalizaion localize);

bool firstPass(const char* fileName, bool firstPass);
int isLabelCheck(char*);
int getOpcode(char*);
int getFunct(int, char*);
Argument getArgument(char*); 

typedef struct Argument {
    int addressingMethod;
    int value;
    bool isLabel;
} Argument;

#endif
