#include "firstPass.h"

int i;
char* tName;
char* regList[]= {"r0","r1","r2","r3","r4","r5","r6","r7","r8","r9","r10","r11","r12","r13","r14","r15"};
/* for debug functions remove later */
void printLabels()
{
    pLabelNode curr = NULL;
    curr = labelsHead;
    printf("\n labels head address: %ld \n B = BASE: O = OFFSET:\n",(long)labelsHead);
    while(curr!= NULL)
    {
        printf(" %s V:%d B:%d O:%d-> \n",(curr->label.name),curr->label.value,curr->label.address,curr->label.offset);
        curr = curr->pNext;
    }
}
void printWords()
{
    pWordNode curr = NULL;
    curr = wordsHead;
    printf("\n words head address: %ld \n",wordsHead);
    while(curr!=NULL)
    {
        printf("label name: %s \tline: %d\tIC: %d \t dest: %c-> \n",(curr->word.name),
               (curr->word.lineNum),curr->word.address, curr->word.labelDest);
        curr = curr->pNext;
    }
}
void printData()
{
    pWordNode curr = NULL;
    curr = datasHead;
    printf("\n data head address: %ld \n",datasHead);
    while(curr!=NULL)
    {
        printf(" %s \tline: %d\tDC: %d \t data: %d-> \n",(curr->word.name),
               (curr->word.lineNum),curr->word.address, (int)curr->word.code.opcode);
        curr = curr->pNext;
    }
}
bool firstPass(const char* fileName, int *ICF, int *DCF)
{
    FILE *fp = NULL;
    char* asFileName;
    /* Important variables for the loop */
    char *token = NULL;
 /*   char *firstToken = NULL;  */          /* check if needed*/
    char *strTolPtr = NULL;
    char *labelName = NULL;
    bool isError = false;
    bool isLabel;
    int isData, isString, funct,ind , opCode, numOfExpectedArgs;
    int lineNum = 0;
    char line[LINE_LENGTH] = {0};
    char arg[31] = {0};
    Word tempWord = {0};
    Word opcodeWord = {0};
    tempWord.are = A; /* All firstPass words get A, so no need to set it every time */
    opcodeWord.are = A; /* All firstPass words get A, so no need to set it every time */


    IC = STARTING_IC;
    DC = 0;

    tName = (char*)calloc(LABEL_LEN,sizeof(char));
    asFileName = (char*)calloc(strlen(fileName) + strlen(".am") + 1, sizeof(char));
   /* firstToken = (char*) calloc(LABEL_LEN,sizeof(char)); */
    labelName = (char*) calloc(LABEL_LEN,sizeof(char));

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
    datasHead = NULL;

    while (fgets(line, LINE_LENGTH, fp) != NULL)
    {
/*        memset(firstToken, '\0',LABEL_LEN); */ /*reset memory */
        memset(tName, '\0',LABEL_LEN);
        memset(labelName, '\0',LABEL_LEN); /*reset memory */
        token = NULL;
        lineNum++;
        numOfExpectedArgs = 0;
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
/*        strncpy(firstToken, token, strlen(token)); */

        if(token[strlen(token)-1] == ':')   /* check if first token is a label */
        {
            isError = labelCheck(asFileName, token, lineNum);
            if (isError) {
                clearLine(line);
                continue;   /*if error found continue next line*/
            }
            isLabel = true;
            strncpy(labelName,token,strlen(token));
            /*if we found a label we want to continue to next argument */
            token = strtok(NULL, " \t\n");
/*            strncpy(firstToken ,token, strlen(token)); */
        }

        /* Check if .string .data .entry .extern and Handle*/
        if (token[0] == '.')
        {
            isData = strcmp(token, ".data");
            isString = strcmp(token, ".string");
            if (isData == 0 || isString == 0) {
                if (isLabel == true) {
                    addLabelNode(labelName, IC + DC, Data, NoneExtOrEnt); /* remembered label from before*/
                    /* strncpy(tempWord.name,labelName,strlen(labelName)); */
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

                            addWordNodeToData(tempWord, DC+IC,0,lineNum);
                            printf(" \n%d , %d",DC,DC+IC);
                            DC++;
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

                        addWordNodeToData(tempWord, DC+IC,0,lineNum);
                        printf(" \n%d , %d",DC,DC+IC);
                        DC++;
                        i++;
                    }
                    /* Now we need to add '\0' to the string */
                    tempWord.code.opcode = 0;

                    addWordNodeToData(tempWord,DC+IC,0,lineNum);
                    printf(" \n%d , %d",DC,DC+IC);
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
                token = strtok(NULL ," \t\n");      /* get label name after .extern declaration */
                if (token == NULL) {
                    isError = true;
                    printError(asFileName, MISSING_LABEL, lineNum);
                    clearLine(line);
                    continue;
                }

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
                addLabelNode(token, 0, NoneDataOrCode, Extern);
                clearLine(line);
                continue;
            } /* not an extern */

            isError = true; /* TODO: change error type*/
            printError(asFileName, INCOMPLETE_CODE, lineNum);
            clearLine(line);
            continue;
        } /* End of handling .string .data .entry .extern */

        /*----------------- handle code ----------------*/
        if (isLabel == true) {
            addLabelNode(/*labelName */tName, IC + DC, Code, NoneExtOrEnt);
        }

        opCode = getOpcode(token);

        if (opCode == -1) {
            isError = true;
            printError(asFileName, UNKNOWN_OPERATION, lineNum);
            clearLine(line);
            continue;
        }

        opcodeWord.code.opcode = opCode;        /* set first word of every command */
        opcodeWord.are = A;
        addWordNodeToCode(opcodeWord, IC, 0, lineNum);
        IC++;
        funct = getFunct(opCode, token);
        token = strtok(NULL, "\n");              /* get the rest of line after the command */
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

        numOfExpectedArgs = getNumOfExpectedArguments(opCode);
        if(numOfExpectedArgs == -1)
        {
            printError(fileName, UNKNOWN_OPERATION,lineNum);
            isError = true;
            continue;
        }
        isError = fillOutArguments(fileName, token, funct, lineNum , numOfExpectedArgs);

        clearLine(line);
    }

    *ICF = (IC - STARTING_IC);
    *DCF = DC;
   /* Need to save a final version of IC and DC here, IDK where though */
    printf("\n");
    printWords();
    printf("\n");
    printData();
    printf("\n");
    printLabels();
    free(asFileName);
    free(tName);
   /* free(firstToken); */

    return isError;
}

/*
A function to check if the first word in the line is a label type, meaning:
    - it starts with an alpha
    - it's all alpha/num
*/
bool labelCheck(char* asFileName, char* label, int lineNum) {
    int  len ,j; /* We will use this to set what situation we're in */
    len = (int)strlen(label);
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
        case ADD:                                       /* covers sub */
            if (strcmp(operation, "add") == 0) return 10;
            if (strcmp(operation, "sub") == 0) return 10;
            break;
        case CLR:                                       /*covers not, inc, dec */
            if (strcmp(operation, "clr") == 0) return 10;
            if (strcmp(operation, "not") == 0) return 11;
            if (strcmp(operation, "inc") == 0) return 12;
            if (strcmp(operation, "dec") == 0) return 13;
            break;
        case JMP:                                       /*covers bne, jsr*/
            if (strcmp(operation, "jmp") == 0) return 10;
            if (strcmp(operation, "bne") == 0) return 11;
            if (strcmp(operation, "jsr") == 0) return 12;
            break;
        default:
            break;
    }
    return 0;
}


/*
A function to get the addressing method and value of an argument.
    - it will check 1 argument and return it's Addressing Method and value
    - it will find errors in the Addressing Method, will return negative ints in addressingMethod for errors
*/
bool fillOutArguments(const char* asFileName, char* argAsStr, unsigned  int funct, int lineNum, int numOfExpectedArgs) {

    char labelName[LABEL_LEN];
    int index;
    bool isError;
    int val[] = {0,0};
    bool isLabel[] = {0,0};
    eAddrresMethod Ad[] = {-1,-1};
    char labels[2][LABEL_LEN];
    int argCount = 0;
    char *firstBracket = NULL;
    char *token = NULL;
    char reg[4] = {0};
    char* temp;
    Word tempWord = {0};
    tempWord.code.operands.funct = funct;
    token = strtok(argAsStr, ", \t\n");
    index = 0;

    while (token != NULL && index < 2){
        memset(labelName, 0, strlen(argAsStr));
        if (token[0] == '#') { /* Immediate */
            argCount++;
            Ad[index] = IMMEDIATE;
            val[index] = (int) strtol(token + 1, &firstBracket, 10);
            if (val[index] < -32767 || val[index] > 32767) {
                printError(asFileName, NUMBER_OUT_OF_BOUND, lineNum);
                val[index] = 0;
                isError = true;
            }
        } else if (isRegName(token)) { /* register direct */
            Ad[index] = DIRECT_REGISTER;
            val[index] = getRegNum(token);
            argCount++;
        } else {
            /* no error, handling a label  */
            firstBracket = strpbrk(argAsStr, "[");
            if (firstBracket != NULL) { /* Index */
                Ad[index] = INDEX;
                /* store reg name for furthur parsing*/
                strncpy(reg,firstBracket+1, strlen(firstBracket)-2);

                /*check if ther is closing bracket */
                firstBracket = strpbrk(argAsStr, "]"); /* find closing brackets */
                if (firstBracket == NULL)
                {
                    printError(asFileName, MISSING_BRACKETS, lineNum);
                    isError = true;
                }

                /* strip label from brackets and store it */
                isLabel[index] = true;
                temp = getLabelFromIndexAddressing(token);
                strncpy(labels[index], temp, LABEL_LEN); /* store label name */

                if (isRegName(reg) == true) /* the index points to register*/
                {
                    val[index] = getRegNum(reg);
                    if (val[index] >= 0 && val[index] < 10)
                    {
                        printError(asFileName, INVALID_USE_OF_REGISTER, lineNum);
                        isError = true;
                    }
                    /* if register legal continue to next step */
                }
                else { /* must have a register name in brackets */
                    printError(asFileName, INVALID_BRACKET_CONTENTS, lineNum);
                    isError = true;
                }
                /* here the brackets content is valid */
            } else { /* Direct */
                Ad[index] = DIRECT;              /* direct to a label */
                val[index] = 0;
                isLabel[index] = true;
                strncpy(labels[i], token, LABEL_LEN);

            }
        }
        index++;
        token = strtok(NULL,"], \t\n");
    }

    if(index < numOfExpectedArgs)
    {
        printError(asFileName, MISSING_ARGUMENTS, lineNum);
        isError = true;
        return isError;
    }else if((index > numOfExpectedArgs) && token != NULL && token[0] != ';') /* if there is extra text after expected argument and it isn't a comment*/
    {
        printError(asFileName, TOO_MANY_ARGUMENTS, lineNum);
        isError = true;
        return isError;
    }

    /*create the second word with the new data*/
    tempWord.are = A;
    tempWord.code.operands.srcAdd = Ad[0];
    if(Ad[0] == DIRECT_REGISTER)
    {
        tempWord.code.operands.srcReg = val[0];
    } else {
        tempWord.code.operands.srcReg = 0;
    }
    if(numOfExpectedArgs == 1)          /* only one argument in command */
    {
        addWordNodeToCode(tempWord, IC, 0, lineNum);
        IC++;
    } else {
        tempWord.code.operands.destAdd = Ad[1];
        if (Ad[1] == DIRECT_REGISTER) {
            tempWord.code.operands.destReg = val[1];
        } else {
            tempWord.code.operands.destReg = 0;
        }
        addWordNodeToCode(tempWord, IC, 0, lineNum);
        IC++;
    }
    if(Ad[0] == DIRECT_REGISTER && Ad[1] == DIRECT_REGISTER)        /*no words to add*/
        return isError;

        /* add data containing words */
    if(Ad[0] == IMMEDIATE)
    {
        tempWord.are = A;
        tempWord.code.opcode = val[0];
        addWordNodeToCode(tempWord, IC, 0, lineNum);
        IC++;
    } else if(Ad[0]==DIRECT || Ad[0] == INDEX)
    {
        tempWord.are = R;
        tempWord.isLabel = isLabel[0];
        strncpy(tempWord.name, labels[0],strlen(labels[0]));
        addWordNodeToCode(tempWord,IC,LABEL_DEST_B,lineNum);
        IC++;
        addWordNodeToCode(tempWord,IC,LABEL_DEST_O,lineNum);
        IC++;
    }

    if(Ad[1] == IMMEDIATE)           /* cant have too immediate */
    {
        tempWord.are = A;
        tempWord.code.opcode = val[1];
        addWordNodeToCode(tempWord, IC, 0, lineNum);
        IC++;
    }
     if(Ad[1] == DIRECT || Ad[1] == INDEX)
    {
        tempWord.are = R;
        tempWord.isLabel = isLabel[1];
        strncpy(tempWord.name, labels[0],strlen(labels[0]));
        addWordNodeToCode(tempWord,IC,LABEL_DEST_B,lineNum);
        IC++;
        addWordNodeToCode(tempWord,IC,LABEL_DEST_O,lineNum);
        IC++;
    }
return isError;
}

/* check if label is registers name */
bool isRegName(const char *label)
{
    int j;
    for( j=0; j < NUM_OF_REGS ; j++)
    {
        if(strcmp(label,(regList[j])) == 0)
        {
            return true;
        }
    }
    return false;
}
/* check if label is registers name */
int getRegNum(const char *label)
{
    int j;
    for( j=0; j < NUM_OF_REGS ; j++)
    {
        if(strcmp(label,(regList[j])) == 0)
        {
            return j;
        }
    }
    return -1;
}
int getNumOfExpectedArguments(eCommands command)
{
    switch(command)
    {
        case MOV:
        case CMP:
        case ADD:           /* covers also the case: SUB */
        case LEA:
            return EXPECTED_TWO_ARGS;
        case CLR:           /* covers also the cases: NOT, INC, DEC */
        case JMP:           /* covers also the cases: BNE, JSR */
        case RED:
        case PRN:
            return EXPECTED_ONE_ARG;
        case RTS:                   /* covers also the cases: STOP */
            return NO_ARGS_EXPECTED;
        default:
            return -1;
    }

}

char* getLabelFromIndexAddressing(char* token)
{
    char * firstBracket = NULL;
    firstBracket = strpbrk(token, "[");
    *firstBracket = '\0';
    return token;
}
