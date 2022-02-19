
#include "firstPass.h"


bool firstPass(const char* fileName, bool firstPass)
{
    FILE *fp = NULL;
    char* asFileName;
    asFileName = (char*)calloc(strlen(fileName) + strlen(".as") + 1, sizeof(char));
    strcat(asFileName, fileName);
    strcat(asFileName, ".as");
    fp = fopen(asFileName, "r");

    if (fp == NULL){
        printError(NULL, MISSING_FILE, 0);
        return true;
    }else {





    }

    free(asFileName);

    return false;
}
