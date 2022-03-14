
#ifndef SECOND_PASS_H
#define SECOND_PASS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "dataStructs.h"

bool readFile(FILE* fp,const char* fileName);        /* reading file */

bool secondPass(const char* fileName, bool firstPass);

bool isEntryInstruction(char *token); /* Input: pointer to string.
                                  * Output: is the string is code instraction or not.
                                  * Discounts: (1) */
bool isDeclaredEntry(char *token); /* Input: receives a token
                                    * Output: true if the token is a name of a declared label, if not returns false. */
bool isLabel(char *token);          /* checks if a given token is a label */

bool addEntryAttribute(const char* fileName, char* label, int lineNum);


#endif
