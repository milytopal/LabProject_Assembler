#include "firstPass.h"

int i;
char* tName;

Label* LabelConstructor(char* name, int val, eDataType dType, eLocalizaion localize)
{
    Label* newL = NULL;
    newL = (Label*)malloc(sizeof (Label));
    strcpy(newL->name, name);
    newL->value = val;
    //newL->.address = (val/16)*16;

    newL->offset = (val % 16);
    newL->address = (val - (newL->offset));

    //newL->.offset = IC - newL->address;
    newL->dataType = dType;
    newL->locationType = localize;

    return newL;
}

bool firstPass(const char* fileName, bool firstPass)
{
    FILE *fp = NULL;
    char* asFileName;
    /* Important variables for the loop */
    char *token = NULL;
    char *firstToken = NULL;
    Label* tempLabel = NULL;

    bool isError = false;
    bool isLabel;
    int isData, isString, funct,ind , opCode;
    int lineNum = 0;
    Word tempWord;
    Word opcodeWord;

    char line[LINE_LENGTH] = {0};
    char arg[31] = {0};


    IC = STARTING_IC;
    DC = 0;



    tName = (char*)calloc(LABEL_LEN,sizeof(char));
    asFileName = (char*)calloc(strlen(fileName) + strlen(".am") + 1, sizeof(char));
    firstToken = (char*) calloc(LABEL_LEN+1,sizeof(char));
    tempLabel = (Label*) calloc(1,sizeof(Label));

    strcpy(asFileName, fileName);
    strcat(asFileName, ".am");
    fp = fopen(asFileName, "r");

    if (fp == NULL){
        printError(NULL, MISSING_FILE, 0);
        return true;
    }

    /* Making sure we can start giving our lists labels and words */
    labelsHead = NULL;
    wordsHead = NULL;

    while (fgets(line, LINE_LENGTH, fp) != NULL)
    {
        /*zero memory */
        memset(firstToken,0,LINE_LENGTH);
        token = NULL;

        lineNum++;
        i = 0;
        isLabel = false;

        if (isEmptyLine(line) == true || line[0] == ';') continue;

        if (line[LINE_LENGTH-2]!= '\0') {
            printError(asFileName, LINE_LIMIT_REACHED, lineNum);
            return true; /* Can't actually check the line */
        }
        token = strtok(line, " \t\n");
        strcpy(firstToken,token);

        switch (isLabelCheck(firstToken /*line*/)) {
            case 0:
                isLabel = true;
                break;
            case 1:
                /* Should be false already */
                break;
            case 2:
                isError = true;
                printError(asFileName, LABEL_LIMIT_REACHED, lineNum);
                continue;
            case 3:
                isError = true;
                printError(asFileName, BAD_LABEL_NAME, lineNum);
                continue;
            case 4:
                isError = true;
                printError(asFileName, LABEL_ALREADY_EXISTS, lineNum);
                continue;
        }/* handle Label */

         while (isSpace(line[i]) != 0) {
             i++;
         }
        /* Check if .string .data .entry .extern and Handle*/
         if (/*line[i]*/firstToken[0] == '.')
         {
            // token = strtok(NULL, " \t\n");         /*get next token ?*/
            isData = strcmp(token, ".data");
            isString = strcmp(token, ".string");
            if (isData == 0 || isString == 0) {
                if (isLabel == true) {
                    tempLabel = LabelConstructor(tName,IC,Data,NoneExtOrEnt);
                    addDataNode(*tempLabel);

                }
                //while (isSpace(line[i]) != 0) {
                //    i++;
                //}

                if (line[i] == '\n') {
                    isError = true;
                    printError(asFileName, NO_ARGUMENTS, lineNum);
                    memset(line,0,LINE_LENGTH);
                    continue;
                }

                if (isData == 0) {
                    /* get argument data */
                    token = strtok(NULL, ", \t\n");
                    if(token == NULL)
                    {
                        printError(fileName, MISSING_PARAMETER,lineNum);
                        continue;
                    }

                    while(token != NULL) {

                        if (isdigit(token[0]) == 0 || token[0] == '-') {
                            strcpy(arg,token);
                        }
                        else
                        {
                            printError(fileName,INVALID_ARGUMENT,lineNum);
                            continue;
                        }
                        ind = atoi(arg);
                        if(ind < -32767 || ind > 32767)
                        {
                            printError(fileName, NUMBER_OUT_OF_BOUND,lineNum);
                            continue;
                        }
                        tempWord.code.opcode = ind;
                        tempWord.are = A;
                        addWordNode(tempWord, (IC + DC));
                        DC++;
                        token = strtok(NULL, ", \t\n");

                    }
                }
                else { /* isString == 0 */
                    i += 7; /* Must have a space */
                    while (line[i] != '\"') {
                        i++;
                    }
                    i++; /* Right here, 'i' points to the first char in the string */
                    while (line[i] != '\"') {
                        tempWord.code.opcode = (unsigned int)line[i];
                        tempWord.are = A;

                        addWordNode(tempWord, (IC+DC));
                        DC++;
                    }
                    /* Now we need to add '\0' to the string */
                    tempWord.code.opcode = 0;
                    tempWord.are = A;

                    addWordNode(tempWord,(IC+DC));
                    DC++;
                }
                memset(line,0,LINE_LENGTH);
                continue;
            }
            else if (strcmp(token, ".entry") == 0) {
                /* Handled on secondPass ! ! */
                memset(line,0,LINE_LENGTH);
                continue;
            }
            else if (strcmp(token, ".extern") == 0) {
                i += strlen(token);

                token = strtok(NULL ," \t\n");      /* get label name after .extern declaration */

                if (token == NULL) {
                    isError = true;
                    printError(asFileName, MISSING_LABEL, lineNum);
                    memset(line,0,LINE_LENGTH);
                    continue;
                }

                ind = 0;
                /* check if externals label length is valid */
                if (strlen(token) >= LABEL_LEN) {
                    isError = true;
                    printError(asFileName, LABEL_LIMIT_REACHED, lineNum);
                    memset(line,0,LINE_LENGTH);
                    continue;
                }
                /* check if extern label name is valid*/
                if (isalpha(token[0]) == 0) {
                    isError = true;
                    printError(asFileName, BAD_LABEL_NAME, lineNum);
                    memset(line,0,LINE_LENGTH);
                    continue;
                }
                /* extern label name is valid here create a label*/
                tempLabel = LabelConstructor(token,0,NoneDataOrCode,Extern);

                addDataNode(*tempLabel);
                memset(line,0,LINE_LENGTH);
                continue;
            } /* not an extern */

            isError = true;
            printError(asFileName, INCOMPLETE_CODE, lineNum);
             memset(line,0,LINE_LENGTH);
             continue;
         } /* Ending of handling .string .data .entry .extern */

         /* everything else */
        if (isLabel == false) {
            i = 0;
            while (isSpace(line[i]) != 0) {
                i++;
            }
        } /* Checked too much, it's prefered we start over with the line */
        else {                          /* we have a label */
            tempLabel = LabelConstructor(tName,IC,Code,NoneExtOrEnt);
            addDataNode(*tempLabel);
            printf("labelName in tempLabel is: %s\n",tempLabel->name );
        }

        /* BUG: needs fixing */
        token = strtok(NULL, " \t\n");
        opCode = getOpcode(token);

        if (opCode == -1) {
            isError = true;
            printError(asFileName, UNKNOWN_OPERATION, lineNum);
            continue;
        }

        funct = getFunct(opCode, token);

        opcodeWord.code.opcode = opCode;
        opcodeWord.are = A;

        addWordNode(opcodeWord, IC);
        IC++;

        if (opCode < 5) { /* 2 args */

        }
        else if (opCode < 14) { /* 1 arg */

        }
        memset(line,'\0',LINE_LENGTH);
        clearLine(line);
    }

    /* Need to save a final version of IC and DC here, IDK where though */

    free(asFileName);
    free(tempLabel);
    return isError;
}

/*
A function to check if the first word in the line is a label type, meaning:
    - it starts with an alpha
    - it's all alpha/num
*/
int isLabelCheck(char* line) {
    int flag ,len ,j; /* We will use this to set what situation we're in */
    char label[31] = {0};
    len = strlen(line);
    flag = 0;
    if(len > LABEL_LEN)
    {
        return 2; /* label name too long */
    }
    if(line[0] == '.')
        return 1;

    if(line[len-1] == ':')
    {
        line[len-1] = '\0';
        len--;
    }

    for(j = 0; j < len; j++)
    {
        if(isalnum(line[j]) == 0)
            return 3;
    }

    printf(" label name: %s",line);
    if (getOpcode(line) != -1) {
        return 3; /* ERROR: Label can't be named that! */
    }

    if (contains(labelsHead, line) != -1) {
        return 4; /* ERROR: Label already exists */
    }

    strcpy(tName, line);
    return 0;
}

int getOpcode(char* op) {
    if(op == NULL) return -1;
    if (strcmp(op, "mov") == 0) return 0;
    if (strcmp(op, "cmp") == 0) return 1;
    if (strcmp(op, "add") == 0) return 2;
    if (strcmp(op, "sub") == 0) return 2;
    if (strcmp(op, "lea") == 0) return 4;
    if (strcmp(op, "clr") == 0) return 5;
    if (strcmp(op, "not") == 0) return 5;
    if (strcmp(op, "inc") == 0) return 5;
    if (strcmp(op, "dec") == 0) return 5;
    if (strcmp(op, "jmp") == 0) return 9;
    if (strcmp(op, "bne") == 0) return 9;
    if (strcmp(op, "jsr") == 0) return 9;
    if (strcmp(op, "red") == 0) return 12;
    if (strcmp(op, "prn") == 0) return 13;
    if (strcmp(op, "rts") == 0) return 14;
    if (strcmp(op, "stop") == 0) return 15;
    return -1;
}

int getFunct(int opCode, char* operation) {
    switch (opCode) {
    case 2:
        if (strcmp(operation, "add") == 0) return 10;
        if (strcmp(operation, "sub") == 0) return 10;
        break;
    case 5:
        if (strcmp(operation, "clr") == 0) return 10;
        if (strcmp(operation, "not") == 0) return 11;
        if (strcmp(operation, "inc") == 0) return 12;
        if (strcmp(operation, "dec") == 0) return 13;
        break;
    case 9:
        if (strcmp(operation, "jmp") == 0) return 10;
        if (strcmp(operation, "bne") == 0) return 11;
        if (strcmp(operation, "jsr") == 0) return 12;
        break;
    default:
        return 0;
        break;
    }
    return 0;
}

int isSpace(char toCheck) { /* that doesn't consider \n as empty, because \n is when our line ends */
    if (toCheck == ' ') return 1;
    if (toCheck == '\t') return 2;
    if (toCheck == '\v') return 3;
    if (toCheck == '\f') return 4;
    if (toCheck == '\r') return 5;
    return 0;
}
