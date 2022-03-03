#include "secondPass.h"


bool secondPass(const char* fileName, bool firstPass)
{
    FILE *fp = NULL;
    char* asFileName;
    asFileName = (char*)calloc(strlen(fileName) + strlen(".am") + 1, sizeof(char));
    strcat(asFileName, fileName);
    strcat(asFileName, ".as");
    fp = fopen(asFileName, "r");

    if (fp == NULL){
        printError(NULL, MISSING_FILE, 0);
        return true;
    }else {
        /* reading file here */


    }

    free(asFileName);

    return false;
}
bool isInstruction(char *token)
{
    if((strcmp(token,".data") == 0) ||(strcmp(token,".extern") == 0)||(strcmp(token,".string") == 0))
    {
        return false;
    }
    return true;
}

bool isDeclaredEntry(char *token) {
    pDataNode curr = NULL;

    if (token == NULL)
        return false;
    curr = labelsHead;
    while ((curr != NULL) && (strcmp(curr->label.name, token) != 0)) {
        curr = curr->pNext;
    }
    if (curr == NULL)            /* reached end of labels list */
    {
        return false;
    }else{
        /* reached label name same as token */
        return true;
    }
}

bool isLabel(char *token)
{
    if(strchr(token,':')!= NULL)
        return true;
    return false;

}




