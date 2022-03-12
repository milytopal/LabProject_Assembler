#include "firstPass.h"

int i;
char* tName;
char regList[]= {"r0","r1","r2","r3","r4","r5","r6","r7","r8","r9","r10","r11","r12","r13","r14","r15"};
/*
Construct a label 
*/

Label* LabelConstructor(char* name, int val, eDataType dType, eLocalizaion localize)
{
    Label* newL = NULL;
    newL = (Label*)malloc(sizeof (Label));
    strcpy(newL->name, name);
    newL->value = val;
    /*newL->.address = (val/16)*16;*/

    newL->offset = (val % 16);
    newL->address = (val - (newL->offset));

    /*newL->.offset = IC - newL->address;*/
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
    Argument newArg = {0};
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
        memset(firstToken,0,LABEL_LEN); /*reset memory */
        token = NULL;
        lineNum++;
        i = 0;
        isLabel = false;

        if (isEmptyLine(line) == true || line[0] == ';') continue;
        /* check for line length excited */
        if (line[LINE_LENGTH-2]!= '\0') {
            printError(asFileName, LINE_LIMIT_REACHED, lineNum);
            memset(line,0,LINE_LENGTH);
            continue; /* Can't actually check the line */
        }
        /* line ok ger first token */
        token = strtok(line, " \t\n");
        strcpy(firstToken,token);

        if(token[strlen(token)-1] == ':')   /* check if first token is a label */
        {
            isLabel = true;
            if(isLabel) {
                isError = labelCheck(asFileName, token, lineNum);
                if (isError) {
                    memset(line,0,LINE_LENGTH);
                    continue;   /*if error found continue next line*/
                }

        /*if we found a label we want to continue to next argument */
            token = strtok(NULL, " \t\n");
            strcpy(firstToken,token);
            }
        }

        /* Check if .string .data .entry .extern and Handle*/
         if (firstToken[0] == '.')
         {
            isData = strcmp(token, ".data");
            isString = strcmp(token, ".string");
            if (isData == 0 || isString == 0) {
                if (isLabel == true) {
                    tempLabel = LabelConstructor(tName,IC,Data,NoneExtOrEnt);
                    addDataNode(*tempLabel);

                }
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
         } /* End of handling .string .data .entry .extern */
         /* handle code */
        if (isLabel == true) {
            tempLabel = LabelConstructor(tName,IC,Code,NoneExtOrEnt);
            addDataNode(*tempLabel);
            printf("labelName in tempLabel is: %s\n",tempLabel->name );
        } /* Checked too much, it's prefered we start over with the line */

        /* BUG: needs fixing */
        token = strtok(NULL, " \t\n");
        opCode = getOpcode(token);
        /* todo: remove later */char mov[] = "r3,W";
        newArg = getArgument(mov);

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
    printf(" label name: %s",label);
    if (getOpcode(label) != -1) {
        printError(asFileName, BAD_LABEL_NAME, lineNum);
        return true; /* ERROR: Label can't be named that! */
    }
    for( j=0; j < NUM_OF_REGS ; j++) /* check if label is registers name */
    {
        if(strcmp(label,(regList+j)) == 0)
        {
            printError(asFileName, BAD_LABEL_NAME, lineNum);
            return true; /* ERROR: Label can't be named that! */
        }
    }
    /*check if label exists already */
    if (contains(labelsHead, label) != -1) {
        printError(asFileName, LABEL_ALREADY_EXISTS, lineNum);
        return true;
        /* ERROR: Label already exists */
    }

/* label name valid - copy label name */
    strcpy(tName, label);
    return 0;
}

/*
Get the opcode of an operation.
    - will return 0 if the given operation in unknown
*/
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
A function to get the addressing method and value of an argumet.
    - it will check 1 argument and return it's AM and value
    - it will find errors in the AM's, will return negative ints in addressingMethod for errors
    - use a switch when calling for this!
*/
Argument getArgument(char* argAsStr) {
    Argument arg;
    arg.isLabel = false;
    if (argAsStr[0] == '#') { /* Immediate */
        arg.value = atoi(argAsStr+1);
        if (arg.value != 0) {            /* PROBLEM: Check if aoti call was successful, will probably be problematic with value 0, is there a better way? */
            arg.addressingMethod = 0;
            return arg;
        }
        else {
            arg.addressingMethod = -1; /* ERROR: Invalid number on immediate AM */
            return arg;
        }
    }

    if (argAsStr[0] == 'r') { /* MIGHT be register direct */
        arg.value = atoi(argAsStr+1);
        if (arg.value >= 0 && arg.value <= 15) {           /* PROBLEM: aoti returns 0 for unsuccessful convertion!!! */
            arg.addressingMethod = 3;
            return arg;
        }
        /* no error, it's probably a label */
    }

    char *firstBracket = strpbrk(argAsStr, '[');
    if (firstBracket != NULL) { /* Index */
        if (firstBracket[1] == 'r') { /* register? */
            if (firstBracket[4] == ']') {/* closed brackets? */
                arg.value = atoi(firstBracket+2);
                if (arg.value >= 10 && arg.value <= 15) {
                    arg.addressingMethod = 2;
                    arg.isLabel = true;
                    return arg;
                }
            } 
        }
        arg.addressingMethod = -21; /* ERROR: Invalid bracket contents on index AM */
        return arg;
    }

    else { /* Direct */
        arg.addressingMethod = 1;
        arg.value = 0;
        arg.isLabel = true;
        return arg;
    }
}
