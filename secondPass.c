#include "secondPass.h"


bool secondPass(const char* fileName, bool firstPass)
{
    FILE *fp = NULL;
    char* asFileName;
    bool isError;
    asFileName = (char*)calloc(strlen(fileName) + strlen(".am") + 1, sizeof(char));
    strcpy(asFileName, fileName);
    strcat(asFileName, ".am");
    fp = fopen(asFileName, "r");

    if (fp == NULL){
        printError(NULL, MISSING_FILE, 0);
        return true;
    }else {
        /* reading file here */
        isError = readFile(fp,fileName);
    }

    free(asFileName);

    return false;
}

bool readFile(FILE* fp, const char* fileName)
{
    char* asFileName;
    /* Important variables for the loop */
    char *token = NULL;
    char *firstToken = NULL;            /* check if needed*/
    char *labelName = NULL;
    bool isError = false;
    bool isLabel;
    int isData, isString, funct,ind , opCode;
    int lineNum = 0;
    char line[LINE_LENGTH] = {0};

    pWordNode currCode = NULL;
    pWordNode currData = NULL;
    Word tempWord;
    Word opcodeWord;
    tempWord.are = A; /* All firstPass words get A, so no need to set it every time */
    opcodeWord.are = A; /* All firstPass words get A, so no need to set it every time */

    currCode = wordsHead;
    currData = datasHead;
/*
    tName = (char*)calloc(LABEL_LEN,sizeof(char));
*/
    firstToken = (char*) calloc(LABEL_LEN,sizeof(char));
    labelName = (char*) calloc(LABEL_LEN,sizeof(char));

    if (fp == NULL){
        printError(NULL, MISSING_FILE, 0);
        return true;
    }

    /* Making sure we can start giving our lists labels and words */

    while (fgets(line, LINE_LENGTH, fp) != NULL) {
        memset(firstToken, '\0', LABEL_LEN); /*reset memory */
        memset(labelName, '\0', LABEL_LEN); /*reset memory */

        token = NULL;
        lineNum++;
        isLabel = false;

        if (isEmptyLine(line) == true || line[0] == ';') continue;
        /* check for line length excited */
        if (line[LINE_LENGTH - 2] != '\0') {
            printError(asFileName, LINE_LIMIT_REACHED, lineNum);
            clearLine(line);
            continue; /* Can't actually check the line */
        }
        /* line ok ger first token */
        token = strtok(line, " \t\n");

        if (token[strlen(token) - 1] == ':')   /* check if first token is a label */
        {
            continue;
        }
        /* Check if .string .data .entry .extern and Handle*/
        if (token[0] == '.') {
            if(isEntryInstruction(token))
            {
                isError = addEntryAttribute(fileName,token,lineNum);
            }else
            {
                continue;
            }
/* handle binary operands here */



        }


    }


}




bool addEntryAttribute(const char* fileName, char* label, int lineNum)
{
    pLabelNode curr = NULL;

    if (labelsHead == NULL)
        return false;
    curr = labelsHead;
    while ((curr != NULL) && (strcmp(curr->label.name, label) != 0)) {
        curr = curr->pNext;
    }
    if (curr == NULL)            /* reached end of labels list */
    {
        printError(fileName,LABEL_DOSNT_EXIST, lineNum);
        return false;
    }else {
        if (curr->label.locationType == Extern) {
            printError(fileName, LABEL_IS_ALREADY_EXTERN, lineNum);
            return false;
        }
    }
        return true;
}



bool isEntryInstruction(char *token)
{
    if((strcmp(token,".data") == 0) ||(strcmp(token,".extern") == 0)||(strcmp(token,".string") == 0))
    {
        return false;
    }
    return true;
}


/* first update entries table and than update the word list accordingly
 * need to add to words the name of labels if there is a label
 * idea: maybe change temporarily the ARE register to flag it as something that needs updating */
bool isDeclaredEntry(char *token) {
    pLabelNode curr = NULL;

    if (token == NULL)
        return false;
    /*curr = labelsHead;*/
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




