#include "firstPass.h"

int i;
char* tName;
char* regList[]= {"r0","r1","r2","r3","r4","r5","r6","r7","r8","r9","r10","r11","r12","r13","r14","r15"};
/* for debug functions remove later */
void printLabels()
{
    pLabelNode curr = NULL;
    curr = labelsHead;
    printf("\n labels head address: %ld \n B = BASE: O = OFFSET:\n",&labelsHead);
    while(curr!= NULL)
    {
        printf(" %s V:%d B:%d O:%d-> ",(curr->label.name),curr->label.value,curr->label.address,curr->label.offset);
        curr = curr->pNext;
    }
}
void printWords()
{
    pWordNode curr = NULL;
    curr = wordsHead;
    printf("\n words head address: %ld \n",&wordsHead);
    while(curr!=NULL)
    {
        printf("  %d -> ",(curr->word.lineNum));
        curr = curr->pNext;
    }
}

bool firstPass(const char* fileName, bool firstPass)
{
    FILE *fp = NULL;
    char* asFileName;
    /* Important variables for the loop */
    char *token = NULL;
    char *firstToken = NULL;
    char *strTolPtr = NULL;
    bool isError = false;
    bool isLabel;
    int isData, isString, funct,ind , opCode;
    int lineNum = 0;
    Argument newArg = {0};
    char line[LINE_LENGTH] = {0};
    char arg[31] = {0};
    bool tempBool = false;
    Word tempWord;
    Word opcodeWord;
    tempWord.are = A; /* All firstPass words get A, so no need to set it every time */
    opcodeWord.are = A; /* All firstPass words get A, so no need to set it every time */


    IC = STARTING_IC;
    DC = 0;

    tName = (char*)calloc(LABEL_LEN,sizeof(char));
    asFileName = (char*)calloc(strlen(fileName) + strlen(".am") + 1, sizeof(char));
    firstToken = (char*) calloc(LABEL_LEN,sizeof(char));
    /*tempLabel = (Label*) calloc(1,sizeof(Label));*/

    strncpy(asFileName, fileName, strlen(fileName));
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
        memset(firstToken, '\0',LABEL_LEN); /*reset memory */
        memset(tName, '\0',LABEL_LEN);
        token = NULL;
        lineNum++;
        i = 0;
        isLabel = false;

        if (isEmptyLine(line) == true || line[0] == ';') continue;
        /* check for line length excited */
        if (line[LINE_LENGTH-2]!= '\0') {
            printError(asFileName, LINE_LIMIT_REACHED, lineNum);
            clearLine(line);
            continue; /* Can't actually check the line */
        }
        /* line ok ger first token */
        token = strtok(line, " \t\n");
        strncpy(firstToken, token, strlen(token));

        if(token[strlen(token)-1] == ':')   /* check if first token is a label */
        {
            isError = labelCheck(asFileName, token, lineNum);
            if (isError) {
                clearLine(line);
                continue;   /*if error found continue next line*/
            }
            isLabel = true;
            /*if we found a label we want to continue to next argument */
            token = strtok(NULL, " \t\n");
            strncpy(firstToken ,token, strlen(token));
        }

        /* Check if .string .data .entry .extern and Handle*/
         if (firstToken[0] == '.')
         {
            isData = strcmp(token, ".data");
            isString = strcmp(token, ".string");
            if (isData == 0 || isString == 0) {
                if (isLabel == true) {
                    addDataNode(tName ,IC ,Data ,NoneExtOrEnt);
                    /*DC++;     needed??? */
                }

                if (isData == 0)
                {
                    /* get argument data */
                    token = strtok(NULL, ", \t\n");
                    if(token == NULL)
                    {
                        printError(fileName, MISSING_PARAMETER,lineNum);
                        isError = true;
                        clearLine(line);
                        continue;
                    }

                    while(token != NULL) {
                        if (isdigit(token[0]) != 0 ||token[0] == '+'|| token[0] == '-')
                        {
                            strncpy(arg,token,strlen(token));
                            ind = (int)strtol(arg, &strTolPtr, 10);
                            if(ind < -32767 || ind > 32767)
                            {
                                printError(fileName, NUMBER_OUT_OF_BOUND,lineNum);
                                isError = true;
                                continue;
                            }
                            tempWord.code.opcode = ind;
                            addWordNode(tempWord, (IC + DC),firstToken,lineNum);
                            DC++;
                            printf("\n%s  in IC: %d in DC: %d total IC+SC:%d", arg,IC,DC,(IC+DC));
                        }
                        else
                        {
                            printError(fileName,INVALID_ARGUMENT,lineNum);
                            isError = true;
                            clearLine(line);
                            break;
                        }
                        token = strtok(NULL, ", \t\n");
                        memset(arg,'\0', strlen(arg)); /* reset memory*/
                    }
                }
                else { /* isString == 0 */
                    i=0;
                    token = strtok(NULL, " \"\t\n"); /* get string data*/
                    while (token[i] != '\0') {
                        tempWord.code.opcode = (unsigned int)token[i];
                        tempWord.are = A;

                        addWordNode(tempWord, (IC + DC),firstToken,lineNum);
                        DC++;
                        i++;
                    }
                    /* Now we need to add '\0' to the string */
                    tempWord.code.opcode = 0;
                    addWordNode(tempWord,(IC + DC),firstToken,lineNum);
                    DC++;
                }
                clearLine(line);
                continue;
            }
            else if (strcmp(token, ".entry") == 0) {
                /* Handled on secondPass ! ! */
                clearLine(line);
                continue;
            }
            else if (strcmp(token, ".extern") == 0) {
                i += strlen(token);

                token = strtok(NULL ," \t\n");      /* get label name after .extern declaration */

                if (token == NULL) {
                    isError = true;
                    printError(asFileName, MISSING_LABEL, lineNum);
                    clearLine(line);
                    continue;
                }

                ind = 0;
                /* check if externals label length is valid */
                if (strlen(token) >= LABEL_LEN) {
                    isError = true;
                    printError(asFileName, LABEL_LIMIT_REACHED, lineNum);
                    clearLine(line);
                    continue;
                }
                /* check if extern label name is valid*/
                if (isalpha(token[0]) == 0) {
                    isError = true;
                    printError(asFileName, BAD_LABEL_NAME, lineNum);
                    clearLine(line);
                    continue;
                }
                /* extern label name is valid here create a label*/
                addDataNode(token,0,NoneDataOrCode,Extern);
                clearLine(line);
                continue;
            } /* not an extern */

            isError = true;
            printError(asFileName, INCOMPLETE_CODE, lineNum);
             clearLine(line);
             continue;
         } /* End of handling .string .data .entry .extern */

         /* handle code */
        if (isLabel == true) {
            addDataNode(tName,IC,Code,NoneExtOrEnt);
        } /* Checked too much, it's prefered we start over with the line */

        opCode = getOpcode(token);

        if (opCode == -1) {
            isError = true;
            printError(asFileName, UNKNOWN_OPERATION, lineNum);
            clearLine(line);
            continue;
        }

        opcodeWord.code.opcode = opCode;
        opcodeWord.are = A;
        addWordNode(opcodeWord, IC,firstToken,lineNum);
        IC++;
        funct = getFunct(opCode, token);

        token = strtok(NULL, ", \t\n");
        if(opCode == STOP)
        {
            if(token != NULL) /* no arguments expected */
            {
                printError(asFileName, TOO_MANY_ARGUMENTS , lineNum);
                isError = true;
                clearLine(line);
            }
            /* no arguments to get */
            continue;
        }else{ /* opcode is not for stop */
            if(token == NULL) /* arguments expected but was not received */
            {
                printError(asFileName,NO_ARGUMENTS,lineNum);
                clearLine(line);
                isError = true;
                continue;
            }
        }

        /*---------------- expected first argument ------------------*/
        newArg = getArgument(fileName,token, lineNum);
if(newArg.isError == true) {
isError = true;
/* printing the error was handled in the getArgument function */
    clearLine(line);
continue;
}
        tempWord.code.operands.funct = funct;
        tempWord.code.operands.destAdd = newArg.addressingMethod;
        tempWord.code.operands.destReg = newArg.value;
        tempWord.code.operands.srcReg = 0;
        tempWord.code.operands.srcAdd = 0;
        tempWord.isLabel = newArg.isLabel;
        if(tempWord.isLabel == true)
            strncpy((tempWord.name),token,LABEL_LEN);
        tempBool = newArg.isLabel;
        addWordNode(tempWord, IC,firstToken,lineNum);
        IC++;

        /*--------------- expected second argument -----------------*/
        if(opCode < 5 && opCode > -1)
        {
            token = strtok(NULL, ", \t\n");
            if (token == NULL) {
                printError(fileName, MISSING_PARAMETER,lineNum);
                isError = true;
                clearLine(line);
                continue;
            }
            newArg = getArgument(asFileName, token, lineNum);
            if(newArg.isError == true) {
                isError = true;
                /* printing the error was handled in the getArgument function */
                clearLine(line);
                continue;
            }

            tempWord.code.operands.srcAdd = newArg.addressingMethod;
            tempWord.code.operands.srcReg = newArg.value;
            tempWord.isLabel = newArg.isLabel;
            if(tempWord.isLabel == true)
                strncpy((tempWord.name),token,LABEL_LEN);
            tempBool = newArg.isLabel;
            addWordNode(tempWord, IC,firstToken,lineNum);
            IC++;
        }
        if (tempWord.isLabel == true) {
            IC += 1;    /*why??*/
        }
        /* no arguments expected */
        token = strtok(NULL, ", \t\n");
        if(token != NULL) /* no arguments expected */
        {
            printError(asFileName, TOO_MANY_ARGUMENTS , lineNum);
            isError = true;
            clearLine(line);
            continue;
        }

        clearLine(line);
    }

    /* Need to save a final version of IC and DC here, IDK where though */
    printf("\n");
    printWords();
    printf("\n");
    printLabels();
    free(asFileName);
    free(tName);
    free(firstToken);

    return isError;
}

/*
A function to check if the first word in the line is a label type, meaning:
    - it starts with an alpha
    - it's all alpha/num
*/
bool labelCheck(char* asFileName, char* label, int lineNum) {
    int flag ,len ,j; /* We will use this to set what situation we're in */
    char temp[LABEL_LEN]={0};
    len = strlen(label);
    flag = 0;
    if(len > LABEL_LEN)
    {
        printError(asFileName, LABEL_LIMIT_REACHED, lineNum);
        return true; /* label name too long */
    }

/* consider leaving it here - clean ':' from label */
    if(label[len-1] == ':')
    {
        label[len-1] = '\0';
        len--;
    }
    for(j = 0; j < len; j++)
    {
        if(isalnum(label[j]) == 0)
        {
            printError(asFileName, BAD_LABEL_NAME, lineNum);
            return true;
        }
    }
    /*check if label is with a saved word - mabe need to add a check if it's a register name */
    if (getOpcode(label) != -1) {
        printError(asFileName, BAD_LABEL_NAME, lineNum);
        return true; /* ERROR: Label can't be named that! */
    }
    /* check if label is registers name */
    if(isRegName(label))
    {
        printError(asFileName, BAD_LABEL_NAME, lineNum);
        return true; /* ERROR: Label can't be named that! */
    }

    /*check if label exists already */
    if (contains(labelsHead, label) != -1) {
        printError(asFileName, LABEL_ALREADY_EXISTS, lineNum);
        return true;
        /* ERROR: Label already exists */
    }

/* label name valid - copy label name */
    strncpy(tName, label, strlen(label));
    return 0;
}

/*
Get the opcode of an operation.
    - will return 0 if the given operation in unknown
*/
eCommands getOpcode(char* op) {
    if(op == NULL) return -1;
    if (strcmp(op, "mov") == 0) return MOV;
    if (strcmp(op, "cmp") == 0) return CMP;
    if (strcmp(op, "add") == 0) return ADD;
    if (strcmp(op, "sub") == 0) return SUB;
    if (strcmp(op, "lea") == 0) return LEA;
    if (strcmp(op, "clr") == 0) return CLR;
    if (strcmp(op, "not") == 0) return NOT;
    if (strcmp(op, "inc") == 0) return INC;
    if (strcmp(op, "dec") == 0) return DEC;
    if (strcmp(op, "jmp") == 0) return JMP;
    if (strcmp(op, "bne") == 0) return BNE;
    if (strcmp(op, "jsr") == 0) return JSR;
    if (strcmp(op, "red") == 0) return RED;
    if (strcmp(op, "prn") == 0) return PRN;
    if (strcmp(op, "rts") == 0) return RTS;
    if (strcmp(op, "stop") == 0) return STOP;
    return -1;
}

/*
Get the funct of an operation.
    - will return 0 if the operation has no funct (which is what the funct should be at that situation)
*/
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


/*
A function to get the addressing method and value of an argument.
    - it will check 1 argument and return it's Addressing Method and value
    - it will find errors in the Addressing Method, will return negative ints in addressingMethod for errors
*/
Argument getArgument(const char* asFileName, char* argAsStr, int lineNum) {
    Argument arg = {0};
    char *firstBracket = NULL;
    char* token = NULL;
    arg.isLabel = false;
    arg.isError = false;
    if (argAsStr[0] == '#') { /* Immediate */
        arg.value = strtol(argAsStr+1, &firstBracket, 10);
        {
            if (arg.value < -32767 || arg.value > 32767) {
                printError(asFileName, NUMBER_OUT_OF_BOUND, lineNum);
                arg.value = 0;
                arg.isError = true;
                return arg;
            }
            arg.addressingMethod = IMMEDIATE;
            return arg;
        }
    }

    if (isRegName(argAsStr)) { /* register direct */
        arg.value = getRegNum(argAsStr);
            arg.addressingMethod = DIRECT_REGISTER;
            return arg;
    }

    /* no error, handling a label  */
    firstBracket = strpbrk(argAsStr, "[");
    if (firstBracket != NULL) { /* Index */
        firstBracket = strpbrk(argAsStr, "]"); /* find closing brackets */
        if(firstBracket == NULL) {
            printError(asFileName, MISSING_BRACKETS, lineNum);
            arg.isError = true;
            return arg;
        }
        /* strip label from brackets and store it */
        token = strtok(argAsStr,"[\n");
        arg.isLabel = true;
        strncpy(arg.labelName,token,LABEL_LEN); /* store label name */
        token = strtok(NULL," ]\t\n");
        if(isRegName(token)== true) /* the index points to register*/
        {
            arg.value = getRegNum(token);
            if (arg.value >= 0 && arg.value <= 10) {
                printError(asFileName, INVALID_USE_OF_REGISTER, lineNum);
                arg.isError = true;
            }
            /* if register ligal continue to next step */
        }else { /* must have a register name in brackets */
            printError(asFileName, INVALID_BRACKET_CONTENTS, lineNum);
            arg.isError = true;
            return arg;
        }
        /* here the brackets content is valid */
        arg.addressingMethod = INDEX;
        return arg;

        } else { /* Direct */
            arg.addressingMethod = DIRECT;
            arg.value = 0;
            arg.isLabel = true;
            strncpy(arg.labelName,argAsStr,LABEL_LEN);
            return arg;
        }
}

bool isRegName(const char *label)
{
    int j;
    for( j=0; j < NUM_OF_REGS ; j++) /* check if label is registers name */
    {
        if(strcmp(label,(regList[j])) == 0)
        {
            return true; /* ERROR: Label can't be named that! */
        }
    }
    return false;
}

int getRegNum(const char *label)
{
    int j;
    for( j=0; j < NUM_OF_REGS ; j++) /* check if label is registers name */
    {
        if(strcmp(label,(regList[j])) == 0)
        {
            return j; /* ERROR: Label can't be named that! */
        }
    }
    return -1;
}
