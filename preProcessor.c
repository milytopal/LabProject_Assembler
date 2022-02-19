
#include "preProcessor.h"
bool PreProcessPass(const char* fileName, bool firstPass)
{
    head = NULL;
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
pMacroNode findMacro(char* name)
{

}

pMacroNode addNewMacro(const char* name)
{
    pMacroNode curr;
    curr = head;
    if(curr == NULL) /* list is empty */
    {
        curr = (pMacroNode) calloc(sizeof(Macro),1);
        strcpy(&(curr->macro.name),name);   /* todo: check if good practice */
    }
    while(curr->pNext != NULL)
    {
        curr = curr->pNext;
    }
    /* at the end of list */
    curr->pNext = (pMacroNode)calloc(sizeof (Macro),1);
    strcpy(curr->pNext->macro.name,name);
    return curr->pNext; /* returning pointer to the new node*/
}
void addLineToMacro(pMacroNode macro, char* line){

    strcat(macro->macro.tokens, line);
}
