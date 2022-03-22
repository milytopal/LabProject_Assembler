
#ifndef FIRST_PASS_H
#define FIRST_PASS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "dataStructs.h"
#include "dataLinkedList.h"
#include "wordLinkedList.h"


bool firstPass(const char* fileName, int *ICF, int *DCF);
bool labelCheck(char*, char*, int);
bool isRegName(const char *label);
int getRegNum(const char *label);
int getNumOfExpectedArguments(eCommands command);
char* getLabelFromIndexAddressing(char* token);

eCommands getOpcode(char*);
int getFunct(int, char*);
bool fillOutArguments(const char*, char*,unsigned int ,int ,int );

#endif
