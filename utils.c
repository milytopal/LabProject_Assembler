
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
    int i;
    for(i = 0 ; i < strlen(line) ; i++)
    {
        if(isspace(line[i]) != 0)
            return false;
    }
    return true;
}
