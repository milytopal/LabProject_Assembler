#include "firstPass.h"

int i;
char* regList[]= {"r0","r1","r2","r3","r4","r5","r6","r7","r8","r9","r10","r11","r12","r13","r14","r15"};
/* for debug functions remove later */

bool firstPass(const char* fileName, int *ICF, int *DCF)
{
    FILE *fp = NULL;
    char* amFileName;
    char *token = NULL;
    char *strTolPtr = NULL;
    char *labelName = NULL;
    bool isError = false;
    bool isLabel, returnStatus;
    int isData, isString, funct,ind , opCode, numOfExpectedArgs;
    int lineNum = 0;
    char line[LINE_LENGTH] = {0};
    char arg[31] = {0};
    Word tempWord = {0};
    Word opcodeWord = {0};
    tempWord.are = A; /* All first Word get A, the determined later */
    opcodeWord.are = A; /* All first Word get A, the determined later */

    IC = STARTING_IC;
    DC = 0;

    amFileName = (char*)calloc(strlen(fileName) + strlen(".am") + 1, sizeof(char));
    labelName = (char*) calloc(LABEL_LEN,sizeof(char));

    strncpy(amFileName, fileName, strlen(fileName));
    strcat(amFileName, ".am");
    fp = fopen(amFileName, "r");

    if (fp == NULL){
        printError(fileName, MISSING_FILE, 0);
        return true;
    }

    /* Making sure we can start giving our lists labels and words */
    labelsHead = NULL;
    wordsHead = NULL;
    datasHead = NULL;
    returnStatus = false;
    while (fgets(line, LINE_LENGTH, fp) != NULL)
    {
        memset(labelName, '\0',LABEL_LEN); /*reset memory */
        token = NULL;
        lineNum++;
        numOfExpectedArgs = 0;
        i = 0;
        isLabel = false;

        if (isEmptyLine(line) == true || line[0] == ';') continue;
        /* check for line length excited */
        if (line[LINE_LENGTH-2]!= '\0') {
            printError(fileName, LINE_LIMIT_REACHED, lineNum);
            clearLine(line);
            continue; /* Can't actually check the line */
        }
        /* line ok ger first token */
        token = strtok(line, " \t\n");

        if(token[strlen(token)-1] == ':')   /* check if first token is a label */
        {
            returnStatus = labelCheck(fileName, token, lineNum);
            if (returnStatus == true) {
                isError = true;
                clearLine(line);
                continue;   /*if error found continue next line*/
            }
            isLabel = true;
            strncpy(labelName,token,strlen(token));
            /*if we found a label we want to continue to next argument */
            token = strtok(NULL, " \t\n");
            if(token == NULL)
            {
                printError(fileName, MISSING_PARAMETER,lineNum);
                isError = true;
                clearLine(line);
                continue;
            }
        }

        /* Check if .string .data .entry .extern and Handle*/
        if (token[0] == '.')
        {
            isData = strcmp(token, ".data");
            isString = strcmp(token, ".string");
            if (isData == 0 || isString == 0) {
                if (isLabel == true) {
                    addLabelNode(labelName, IC + DC, Data, NoneExtOrEnt); /* remembered label from before*/
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

                    while(token != NULL && token[0] != ';') {
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
                    if(token == NULL)
                    {
                        printError(fileName, MISSING_PARAMETER,lineNum);
                        isError = true;
                        clearLine(line);
                        continue;
                    }
                    while (token[i] != '\0' ) {
                        tempWord.code.opcode = (unsigned int)token[i];
                        tempWord.are = A;
                        addWordNodeToData(tempWord, DC+IC,0,lineNum);
                        DC++;
                        i++;
                    }
                    /* Now we need to add '\0' to the string */
                    tempWord.code.opcode = 0;

                    addWordNodeToData(tempWord,DC+IC,0,lineNum);
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
                    printError(fileName, MISSING_LABEL, lineNum);
                    clearLine(line);
                    continue;
                }

                returnStatus = labelCheck(fileName, token, lineNum);
                if (returnStatus == true) {     /*if error found continue next line*/
                    isError = true;
                    clearLine(line);
                    continue;
                }

                /* extern label name is valid here and wasnt declared before create a label */
                addLabelNode(token, 0, NoneDataOrCode, Extern);
                /* check if more text after label */
                token = strtok(NULL ," \t\n");      /* get label name after .extern declaration */
                if(token != NULL && token[0] !=';')
                {
                    printError(fileName,EXTRA_TEXT_AT_END, lineNum);
                    isError = true;
                    clearLine(line);
                    continue;
                }
                clearLine(line);
                continue;
            } /* not an extern */
            /* not a .data .string .extern or .entry*/
            isError = true;
            printError(fileName, INCOMPLETE_CODE, lineNum);
            clearLine(line);
            continue;
        } /* End of handling .string .data .entry .extern */

        /*----------------- handle code ----------------*/
        if (isLabel == true) {
            addLabelNode(labelName /*tName*/, IC + DC, Code, NoneExtOrEnt);
        }

        opCode = getOpcode(token);

        if (opCode == -1) {
            isError = true;
            printError(fileName, UNKNOWN_OPERATION, lineNum);
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
            if(token != NULL && token[0] != ';') /* no arguments expected */
            {
                printError(fileName, TOO_MANY_ARGUMENTS , lineNum);
                isError = true;
                clearLine(line);
            }
            /* no arguments to get */
            continue;
        }else{ /* opcode is not for stop */
            if(token == NULL) /* arguments expected but was not received */
            {
                printError(fileName,NO_ARGUMENTS,lineNum);
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
            clearLine(line);
            continue;
        }
        returnStatus = fillOutArguments(fileName, token, funct, lineNum , numOfExpectedArgs);
        if(returnStatus == true)
        {
            /* error report handled in function */
            isError = true;
            clearLine(line);
            continue;
        }

        clearLine(line);
    }

    *ICF = (IC - STARTING_IC);
    *DCF = DC;

    free(amFileName);
    free(labelName);
    return isError;
}

/*
A function to check if the first word in the line is a label type, meaning:
    - it starts with an alpha
    - it's all alpha/num
*/
bool labelCheck(const char* fileName, char* label, int lineNum) {
    int  len ,j; /* We will use this to set what situation we're in */
    len = (int)strlen(label);
    if(len > LABEL_LEN)
    {
        printError(fileName, LABEL_LIMIT_REACHED, lineNum);
        return true; /* label name too long */
    }
/* consider leaving it here - clean ':' from label */
    if(label[len-1] == ':')
    {
        label[len-1] = '\0';
        len--;
    }
    if(isalpha(label[0]) == 0)
    {
        printError(fileName,BAD_LABEL_NAME,lineNum);
        return true;
    }
    for(j = 0; j < len; j++)
    {
        if(isalnum(label[j]) == 0)
        {
            printError(fileName, BAD_LABEL_NAME, lineNum);
            return true;
        }
    }
    /*check if label is with a saved word - mabe need to add a check if it's a register name */
    if (getOpcode(label) != -1) {
        printError(fileName, USE_OF_SAVED_WORD, lineNum);
        return true; /* ERROR: Label can't be named that! */
    }
    /* check if label is registers name */
    if(isRegName(label))
    {
        printError(fileName, USE_OF_SAVED_WORD, lineNum);
        return true; /* ERROR: Label can't be named that! */
    }

    /*check if label exists already */
    if (wasTheLabelDeclared(label) == true) {
        printError(fileName, LABEL_ALREADY_EXISTS, lineNum);
        return true;
        /* ERROR: Label already exists */
    }
    return false;
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
            if (strcmp(operation, "sub") == 0) return 11;
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
    bool isError = false;
    int val[] = {0,0};
    bool isLabel[] = {0,0};
    eAddrresMethod Ad[] = {-1,-1};
    char labels[2][LABEL_LEN];
    int argCount = 0;
    char *firstBracket = NULL;
    char *secondBracket = NULL;
    char *token = NULL;
    char reg[4] = {0};
    char* temp;
    Word tempWord = {0};
    tempWord.code.operands.funct = funct;
    token = strtok(argAsStr, ", \t\n");
    index = 0;

    while (token != NULL && index < 2 && token[0] != ';'){
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
                /* here stores with closing bracket, witch will be striped later*/
                strncpy(reg,firstBracket+1, 4);

                /*check if ther is closing bracket */
                secondBracket = strpbrk(argAsStr, "]"); /* find closing brackets */
                if (secondBracket == NULL)
                {
                    printError(asFileName, MISSING_BRACKETS, lineNum);
                    isError = true;
                }else
                {
                    /*there is closing bracket*/
                    reg[secondBracket - firstBracket - 1] = '\0';
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

    if(numOfExpectedArgs == 1)          /* only one argument in command - operates as destination */
    {
        tempWord.code.operands.destAdd = Ad[0];
        if(Ad[0] == DIRECT_REGISTER || Ad[0] == INDEX)
        {
            tempWord.code.operands.destReg = val[0];
        } else {
            tempWord.code.operands.destReg = 0;
        }
        addWordNodeToCode(tempWord, IC, 0, lineNum);
        IC++;
    }else  if(numOfExpectedArgs == 2) {
        tempWord.code.operands.srcAdd = Ad[0];
        if (Ad[0] == DIRECT_REGISTER || Ad[0] == INDEX) {
            tempWord.code.operands.srcReg = val[0];
        } else {
            tempWord.code.operands.srcReg = 0;
        }
        tempWord.code.operands.destAdd = Ad[1];
        if (Ad[1] == DIRECT_REGISTER || Ad[1] == INDEX) {
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
    //memset(tempWord.name, 0, LABEL_LEN);
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
    //memset(tempWord.name, 0, strlen(tempWord.name));
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
int getRegNum(const char *reg)
{
    int j;
    for( j=0; j < NUM_OF_REGS ; j++)
    {
        if(strcmp(reg,(regList[j])) == 0)
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
/* strip the brackets from the label */
char* getLabelFromIndexAddressing(char* token)
{
    char * firstBracket = NULL;
    firstBracket = strpbrk(token, "[");
    *firstBracket = '\0';
    return token;
}
