#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include "preProcessor.h"
bool PreProcessPass(const char* fileName)
{
    FILE *fp = NULL;
    FILE *newFp = NULL;
    char* asFileName;
    char* amFileName;
    char cwd[LINE_LENGTH];      /*todo : remove later*/
    fp = NULL; newFp = NULL;
    head = NULL;
    asFileName = (char*)calloc(strlen(fileName) + strlen(".as") + 1, sizeof(char));
    amFileName = (char*)calloc(strlen(fileName) + strlen(".am") + 1, sizeof(char));
    strncpy(asFileName, fileName, strlen(fileName));
    strcat(asFileName, ".as");

    if (getcwd(cwd, sizeof(cwd)) != NULL) {     /*todo : remove later*/
        printf("Current working dir: %s\n", cwd);
    } else {
        perror("getcwd() error");
    }                                           /*todo : remove later*/

    fp = fopen(asFileName, "r");

    if (fp == NULL){
        printError(asFileName, MISSING_FILE, 0);
        printf("couldnt open file: %s\n ",asFileName);
        return true;
    }
    strncpy(amFileName, fileName, strlen(fileName));
    strcat(amFileName, ".am");
    newFp = fopen(amFileName, "w");

    if(newFp == NULL){
        printError(NULL, MISSING_FILE, 0);
        fclose(fp);
        return true;
    }

    foo(fp, newFp);
    fclose(fp);
    fclose(newFp);

    free(asFileName);
    return false;
}

int cleanLine(char* line)
{
    int i = 0;
    while(isspace(line[i]) == 0)
    {
        i++;
    }
    return i;
}

bool isMacro(char *line, int i){
    if(strncmp(line, "macro", 5) == 0){
        return true;
    }

    return false;
}

bool foo(FILE *fp, FILE * newFp)
{
    char line[LINE_LENGTH] = {0};
    /* char cpyLine[LINE_LENGTH] = {0}; */
    char* cpyLine = NULL;
    char macro_name[31] = {0};
    char* token = NULL;
    pMacroNode newMacro = NULL;
    pMacroNode macro = NULL;
    bool insideMacro = false;

    cpyLine = (char*)calloc(LINE_LENGTH, sizeof(char));

    while(fgets(line, LINE_LENGTH, fp) != NULL){
        if(line[LINE_LENGTH-2]!= '\0')
            continue;
        strncpy(cpyLine, line, strlen(line));

        if(strstr(line, "endm") != NULL){
            insideMacro = false;
            clearLine(line);
            continue;
        }

        if(insideMacro == true){
            addLineToMacro(newMacro, line);
            clearLine(line);
            continue;
        }

        if(strstr(line, "macro") != NULL){ /*  find macro statement */
            token = strtok(cpyLine, " \t\n");
            token = strtok(NULL, " \t\n");
            insideMacro = true;
            strncpy(macro_name, token, strlen(token));
            newMacro = addNewMacro(macro_name);
            clearLine(line);
            continue;
        }

        macro = findMacro(strtok(cpyLine, " \t\n")); /* finds the macro object acording to macro name*/
        if(macro != NULL){
            /* write macro to file */
            writeMacroToFile(macro,newFp);
            clearLine(line);
            continue;
        }
        fputs(line,newFp);
        clearLine(line);
    }
    deleteMacroList(head);
    /* todo delete list of macros? */
    return true;
}

pMacroNode findMacro( char* name)
{
    pMacroNode curr = head;
    while(curr != NULL){
        if(strcmp(name,(curr->macro.name)) == 0){
            return curr;
        }
        curr = curr->pNext;
    }
    return curr;
}

pMacroNode addNewMacro(const char* name)
{
    /* return NULL; */
    pMacroNode curr = NULL;
    if(head == NULL) /* list is empty */
    {
        head = (pMacroNode) calloc(sizeof(MacroNode),1);
        strncpy((head->macro.name),name, strlen(name));   /* todo: check if good practice */
        head->macro.tokenList = NULL;
        head->pNext = NULL;
    }else {
        /* at start of list */
        curr = (pMacroNode) calloc(sizeof(MacroNode), 1);
        strncpy((curr->macro.name), name, strlen(name));
        curr->pNext = head;
        curr->macro.tokenList = NULL;
        head = curr;
    }
    return head; /* returning pointer to the new node*/
}
/* add new token the tokens list of a given macro */
void addLineToMacro(pMacroNode macro, char* line){

    pTokenNode curr = NULL;
    if(macro->macro.tokenList == NULL)
    {
        macro->macro.tokenList = (pTokenNode)calloc(LINE_LENGTH , sizeof (TokenNode));
        strncpy(macro->macro.tokenList->token ,line, strlen(line));
    }
    else {
        curr = macro->macro.tokenList;
        while (curr->pNext != NULL) {
            curr = macro->macro.tokenList->pNext;
        }
        curr->pNext = (pTokenNode) calloc (1 , sizeof (TokenNode));
        strncpy(curr->pNext->token,line,strlen(line));
        curr = curr->pNext;
        curr = NULL;
    }

}
/* write all the tokens in the token list of the macro from head to end */
bool writeMacroToFile(pMacroNode macro ,FILE *file)
{
    pTokenNode  curr = NULL;
    if(macro == NULL || file == NULL )
    {
        return false;
    }
    curr = macro->macro.tokenList;
    while(curr != NULL)
    {
        fputs(curr->token,file);
        curr = curr->pNext;
    }
    return true;
}

/* delete the list in the end of process to free memory */
void deleteMacroList(pMacroNode head ) {
    pMacroNode nextMacro = NULL;
    pTokenNode nextToken = NULL;
    /* nextMacro = head; */
    while(head != NULL)
    {
        nextMacro = head->pNext;

        /* delete token list in the macro */
        while(head->macro.tokenList != NULL) {
            nextToken=head->macro.tokenList->pNext;
            free(head->macro.tokenList);
            head->macro.tokenList = nextToken;
        }

        free(head);
        head = nextMacro;

    }
}
