
#include "utils.h"

void printError(const char* currFileName, eErrorCode errCode, int lineNum)
{
    switch (errCode) {
        printf("[ERROR]: ");
        case MISSING_FILE_NAME:
            printf("Missing file Name  \n");
            break;
        case MISSING_FILE:
            printf("The File \"%s\" is Was Not Found\n",currFileName);
            break;
        case LINE_LIMIT_REACHED:
            printf("in file: %s line: %d Line Limit Reached Over 80 Chars\n",currFileName,lineNum);
            break;
        case LABEL_LIMIT_REACHED:
            printf("in file: %s line: %d Label Limit Reached Over 31 Chars\n",currFileName,lineNum);
            break;
        case BAD_LABEL_NAME:
            printf("in file: %s line: %d The declared label can't be named that\n",currFileName,lineNum);
            break;
        case FAILED_TO_CREATE:
            printf("in file: %s line: %d Failed to create final file\n",currFileName,lineNum);
            break;
        case NO_ARGUMENTS:
            printf("in file: %s line: %d No arguments are provided\n",currFileName,lineNum);
            break;
        case INCOMPLETE_CODE:
            printf("in file: %s line: %d Code line isn't complete\n",currFileName,lineNum);
            break;
        case MISSING_LABEL:
            printf("in file: %s line: %d Missing label after .extern\n",currFileName,lineNum);
            break;
        case MISSING_PARAMETER:
            printf("in file: %s line: %d A parameter is missing!\n",currFileName,lineNum);
            break;
        case LABEL_ALREADY_EXISTS:
            printf("in file: %s line: %d Declared label was declared already\n",currFileName,lineNum);
            break;
        case UNKNOWN_OPERATION:
            printf("in file: %s line: %d Unknown operation\n",currFileName,lineNum);
            break;
        case INVALID_ARGUMENT:
            printf("in file: %s line: %d Argument isn't a number\n",currFileName,lineNum);
            break;
        case NUMBER_OUT_OF_BOUND:
            printf("in file: %s line: %d Given number is out of bounds -32767 < x < 32767\n",currFileName,lineNum);
            break;
        case INVALID_BRACKET_CONTENTS:
            printf("in file: %s line: %d The bracket contents are invalid, must be a register between r10 and r15\n",currFileName,lineNum);
            break;
        case TOO_MANY_ARGUMENTS:
            printf("in file: %s line: %d Too many arguments were given to this operation\n",currFileName,lineNum);
            break;
        case INVALID_USE_OF_REGISTER:
            printf("in file: %s line: %d Given register is out of bounds in this context, 10 >= r >= 15\n",currFileName,lineNum);
            break;
        case MISSING_BRACKETS:
            printf("in file: %s line: %d Missing brackets\n",currFileName,lineNum);
            break;
        case LABEL_IS_ALREADY_EXTERN:
            printf("in file: %s line: %d Label is already extern\n",currFileName,lineNum);
            break;
        case LABEL_DOSNT_EXIST:
            printf("in file: %s line: %d Given label doesn't exist!\n",currFileName,lineNum);
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