
#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "dataStructs.h"

bool PreProcessPass(const char* fileName, bool firstPass);
pMacroNode addNewMacro(const char* name);
void addLineToMacro(pMacroNode macro, char* line);
pMacroNode findMacro(char* name);

pMacroNode head;

#endif PREPROCESSOR_H
