
#ifndef SECOND_PASS_H
#define SECOND_PASS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "dataStructs.h"

bool readFile(FILE* fp, char* fileName);        /* reading file */

bool secondPass(char* fileName, int *ICF, int *DCF);

bool isEntryInstruction(char *token); /* Input: pointer to string.
                                  * Output: is the string is code instraction or not.
                                  * Discounts: (1) */

bool addEntryAttribute(const char* fileName, char* label, int lineNum);
void UpdateWordsListNodes();


#endif
