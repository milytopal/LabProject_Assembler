#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include "preProcessor.h"
bool PreProcessPass(const char* fileName, bool firstPass)
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
    strcpy(asFileName, fileName);
    strcat(asFileName, ".as");

    if (getcwd(cwd, sizeof(cwd)) != NULL) {     /*todo : remove later*/
        printf("Current working dir: %s\n", cwd);
    } else {
        perror("getcwd() error");
    }                                           /*todo : remove later*/

    fp = fopen(asFileName, "r");

    if (fp == NULL){
        printError(NULL, MISSING_FILE, 0);
        printf("couldnt open file: %s\n ",asFileName);
        return true;
    }
    strcpy(amFileName, fileName);
    strcat(amFileName, ".am");
    newFp = fopen(amFileName, "a");

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
    //char cpyLine[LINE_LENGTH] = {0};
    char* cpyLine = NULL;
    char *rest_of_line = NULL;
    char macro_name[31] = {0};
    char* token = NULL;
    pMacroNode newMacro = NULL;
    pMacroNode macro = NULL;
    bool insideMacro = false;


    cpyLine = (char*)calloc(LINE_LENGTH, sizeof(char));
    printf("inside foo\n");
    while(fgets(line, LINE_LENGTH, fp) != NULL){
        printf("inside foo loop\n");
        if(line[LINE_LENGTH-2]!= '\0')
            continue;
        strcpy(cpyLine, line);

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

        if(strstr(line, "macro") != NULL){ //macro statement
            printf("found macro line is : %s\n", cpyLine);              /* todo: delete later*/
            token = strtok(cpyLine, " \t\n");
            printf("found macro 2 rest of line : %s  cpyLine : %s\n", token, cpyLine); /* todo: delete later*/
            token = strtok(NULL, " \t\n");
            printf("macro name %s", token);         /* todo: delete later*/
            insideMacro = true;
            strcpy(macro_name, token);
            newMacro = addNewMacro(macro_name);
            clearLine(line);
            continue;
        }

        macro = findMacro(strtok(cpyLine, " \t\n"));
        if(macro != NULL){
            //write macro to file
            clearLine(line);
            continue;
        }
        printf("inside foo loop before fputs line is : %s\n", line);
        fputs(line,newFp);
        clearLine(line);
    }
    //todo read file
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
    //return NULL;
    pMacroNode curr = NULL;
    if(head == NULL) /* list is empty */
    {
        printf("in add new macro head == NULL \n");
        head = (pMacroNode) calloc(sizeof(MacroNode),1);
        strcpy((head->macro.name),name);   /* todo: check if good practice */
        head->macro.tokenList = NULL;
        head->pNext = NULL;
    }else {
        /* at start of list */
        curr = (pMacroNode) calloc(sizeof(MacroNode), 1);
        strcpy((curr->macro.name), name);
        curr->pNext = head;
        curr->macro.tokenList = NULL;
        head = curr;
    }
    return head; /* returning pointer to the new node*/
}
void addLineToMacro(pMacroNode macro, char* line){

    //strcat(macro->macro.tokens, line);
}
