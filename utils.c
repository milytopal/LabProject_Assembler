#include "utils.h"

void printError(const char* currFileName, eErrorCode errCode, int lineNum)
{
    printf("[ERROR]: ");
    switch (errCode) {
        case FAILED_TO_OPEN:
            printf("Failed to Open File Named: \"%s\" Was Not Found\n", currFileName);
            break;
        case MISSING_FILE:
            printf("File \"%s\" Was Not Found\n", currFileName);
            break;
        case LINE_LIMIT_REACHED:
            printf("In File: %s, Line: %d - Line Limit Reached Over 80 Chars\n",currFileName,lineNum);
            break;
        case LABEL_LIMIT_REACHED:
            printf("In File: %s, Line: %d - Label Limit Reached Over 31 Chars\n",currFileName,lineNum);
            break;
        case BAD_LABEL_NAME:
            printf("In File: %s, Line: %d - Label Name Dose Not Meet Requirements\n",currFileName,lineNum);
            break;
        case FAILED_TO_CREATE:
            printf("In File: %s, Line: %d - Failed to Create Final File\n",currFileName,lineNum);
            break;
        case NO_ARGUMENTS:
            printf("In File: %s, Line: %d - No Arguments are Provided\n",currFileName,lineNum);
            break;
        case INCOMPLETE_CODE:
            printf("In File: %s, Line: %d - Code Line isn't Complete\n",currFileName,lineNum);
            break;
        case MISSING_LABEL:
            printf("In File: %s, Line: %d - Missing Label Name After .extern\n",currFileName,lineNum);
            break;
        case MISSING_PARAMETER:
            printf("In File: %s, Line: %d - A parameter is missing!\n",currFileName,lineNum);
            break;
        case LABEL_ALREADY_EXISTS:
            printf("In File: %s, Line: %d - Declared Label Was Declared Already\n",currFileName,lineNum);
            break;
        case UNKNOWN_OPERATION:
            printf("In File: %s, Line: %d - Unknown operation\n",currFileName,lineNum);
            break;
        case INVALID_ARGUMENT:
            printf("In File: %s, Line: %d - Argument isn't a Number\n",currFileName,lineNum);
            break;
        case NUMBER_OUT_OF_BOUND:
            printf("In File: %s, Line: %d - Given Number is Out of Bounds -32767 < x < 32767\n",currFileName,lineNum);
            break;
        case INVALID_BRACKET_CONTENTS:
            printf("In File: %s, Line: %d - The Bracket Contents are Invalid, Must be a Register Between r10 and r15\n",currFileName,lineNum);
            break;
        case TOO_MANY_ARGUMENTS:
            printf("In File: %s, Line: %d - Too Many Arguments Were Given to This Operation\n",currFileName,lineNum);
            break;
        case INVALID_USE_OF_REGISTER:
            printf("In File: %s, Line: %d - Given Register is Out of Bounds in This Context,  Must be a Register Between 10 and 15\n",currFileName,lineNum);
            break;
        case MISSING_BRACKETS:
            printf("In File: %s, Line: %d - Missing Brackets\n",currFileName,lineNum);
            break;
        case LABEL_IS_ALREADY_EXTERN:
            printf("In File: %s, Line: %d - Label is Already Extern\n",currFileName,lineNum);
            break;
        case LABEL_DOSNT_EXIST:
            printf("In File: %s, Line: %d - Given Label Doesn't Exist\n",currFileName,lineNum);
            break;
        case MISSING_ARGUMENTS:
            printf("In File: %s, Line: %d - Less Arguments Were Given Than Expected\n",currFileName,lineNum);
            break;
        case LABEL_WASNT_USED_AS_ENTRY:
            printf("In File: %s, Line: %d - Label Wasn't Used as Entry in Code or Data Segments\n",currFileName,lineNum);
            break;
        case EXTRA_TEXT_AT_END:
            printf("In File: %s, Line: %d - Extra Text at End of Declaration\n",currFileName,lineNum);
            break;
        case USE_OF_SAVED_WORD:
            printf("In File: %s, Line: %d - Saved Word Cannot be Used as Label Name\n",currFileName,lineNum);
            break;
        case EXTRA_COMMA_AT_END:
            printf("In File: %s, Line: %d - Extra Comma At End Of Instruction\n",currFileName,lineNum);
            break;
        case MAX_RAM_EXCITED:
            printf("In File: %s - Out Of Memory, Memory Limited To 8092 Words \n",currFileName);
            break;
        default:
            printf("In File: %s, Line: %d - an Unexpected Error Occurred \n",currFileName,lineNum);
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

