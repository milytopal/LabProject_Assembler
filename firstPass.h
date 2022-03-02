
#ifndef FIRST_PASS_H
#define FIRST_PASS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "dataStructs.h"

bool firstPass(const char* fileName, bool firstPass);
static int isLabelCheck(char*);
static int getOpcode(char*);



pWordNode list;
//Label *labels;

#endif
