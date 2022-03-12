
#include "utils.h"

void printError(const char* currFileName, eErrorCode errCode, int lineNum)
{
    switch (errCode) {
        case MISSING_FILE_NAME:
            printf("Missing file Name  \n");
            break;
        case MISSING_FILE:
            printf("The File \"%s\" is Was Not Found\n",currFileName);
            break;
        case LINE_LIMIT_REACHED:
            printf("in file: %s line: %d Line Limit Reached Over 80 Chars",currFileName,lineNum);
            break;
    }
}
bool isEmptyLine(char *line)
{
    int i, len;
    len = strlen(line);
    for(i = 0 ; i < len ; i++)
    {
        if(isspace(line[i]) == 0)
            return false;
    }
    return true;
}

void clearLine(char* line)
{
    int i;
    for(i = 0 ; i < strlen(line) ; i++)
    {
        line[i] = '\0';
    }
}

/*
Replacement for isspace() method because isspace() counts '\n' as a space (which is end of line)
*/
int isSpace(char toCheck) { 
    if (toCheck == ' ') return 1;
    if (toCheck == '\t') return 2;
    if (toCheck == '\v') return 3;
    if (toCheck == '\f') return 4;
    if (toCheck == '\r') return 5;
    return 0;
}