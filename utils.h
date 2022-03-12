
#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "dataStructs.h"

void printError(const char* currFileName, eErrorCode errCode, int lineNum);
bool isEmptyLine(char *line);
/* clear the array holding the line */
void clearLine(char *line);
int isSpace(char);

#endif
