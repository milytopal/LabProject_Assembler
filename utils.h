
#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "dataStructs.h"

void printError(const char* currFileName, eErrorCode errCode, int lineNum);
/* check if a line is empty */
bool isEmptyLine(char *line);

/* clear the array holding the line */
void clearLine(char *line);


#endif
