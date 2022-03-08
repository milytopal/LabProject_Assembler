    #include "firstPass.h"

    int i;
    char* tName;

    bool firstPass(const char* fileName, bool firstPass)
    {
        FILE *fp = NULL;
        char* asFileName;
        char *token = NULL;

        tName = (char*)calloc(LABEL_LEN,sizeof(char));
        asFileName = (char*)calloc(strlen(fileName) + strlen(".am") + 1, sizeof(char));

        strcpy(asFileName, fileName);
        strcat(asFileName, ".am");
        fp = fopen(asFileName, "r");

        if (fp == NULL){
            printError(NULL, MISSING_FILE, 0);
            return true;
        }

        /* Making sure we can start giving our lists labels and words */
        labels = NULL;
        words = NULL;

        /* Important variables for the loop */
        int IC = STARTING_IC;
        int DC = 0;
        bool isError = false;
        bool isLabel;

        int n = 0;
        char line[LINE_LENGTH];

        while (fgets(line, LINE_LENGTH, fp) != NULL) {
            n++;
            i = 0;
            isLabel = false;

            if (isEmptyLine(line) == true || line[0] == ';') continue;

            if (line[LINE_LENGTH-2]!= '\0') {
                printError(asFileName, LINE_LIMIT_REACHED, n);
                return true; /* Can't actually check the line */
            }

            switch (isLabelCheck(line)) {
                case 0:
                    isLabel = true;
                    break;
                case 1:
                    /* Should be false already */
                    break;
                case 2:
                    isError = true;
                    printError(asFileName, LABEL_LIMIT_REACHED, n);
                    continue;
                    break;
                case 3:
                    isError = true;
                    printError(asFileName, BAD_LABEL_NAME, n);
                    continue;
                    break;
                case 4:
                    isError = true;
                    printError(asFileName, LABEL_ALREADY_EXISTS, n);
                    continue;
                    break;
            }

             while (isSpace(line[i]) != 0) {
                 i++;
             }
            /* Check if .string .data .entry .extern and Handle*/
             if (line[i] == '.')
             {
                 token = strtok(line, " \t\n");
                int isData = strcmp(token, ".data");
                int isString = strcmp(token, ".string");
                if (isData == 0 || isString == 0) {
                    if (isLabel == true) {
                        Label tempLabel = {0};
                        strcpy(tempLabel.name, tName);
                        tempLabel.value = IC;
                        tempLabel.address = (IC/16)*16;
                        tempLabel.offset = IC - tempLabel.address;
                        tempLabel.dataType = Data;
                        tempLabel.locationType = NoneExtOrEnt;

                        addDataNode(&labels, tempLabel);

                        while (line[i] != ':') {
                            i++;
                        }
                        i++;
                    }
                    while (isSpace(line[i]) != 0) {
                        i++;
                    }

                    if (line[i] == '\n') {
                        isError = true;
                        printError(asFileName, NO_ARGUMENTS, n);
                        memset(line,0,LINE_LENGTH);
                        continue;
                    }

                    if (isData == 0) {
                        i += 6; /* Must have a space */
                        while (line[i] != '\n') {
                            char arg[10];
                            int ind = 0;
                            while (isdigit(line[i]) == 0 || line[i] == '+' || line[i] == '-') {
                                arg[ind++] = line[i++];
                            }
                            ind = atoi(arg);

                            Word tempWord;
                            tempWord.code.opcode = ind;
                            tempWord.are = A;

                            addWordNode(&words, tempWord, (IC+DC));
                            DC++;
                            while (isSpace(line[i]) != 0 || line[i] == ',') {
                                i++;
                            }
                        }
                    }
                    else { /* isString == 0 */
                        i += 7; /* Must have a space */
                        while (line[i] != '\"') {
                            i++;
                        }
                        i++; /* Right here, 'i' points to the first char in the string */
                        while (line[i] != '\"') {
                            Word tempWord;
                            tempWord.code.opcode = line[i];
                            tempWord.are = A;

                            addWordNode(&words, tempWord, (IC+DC));
                            DC++;
                        }
                        /* Now we need to add '\0' to the string */
                        Word tempWord;
                        tempWord.code.opcode = 0;
                        tempWord.are = A;

                        addWordNode(&words, tempWord,(IC+DC));
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
                    Label tempLabel = {0};
                    char name[31] = {0};
                    /* while (isSpace(line[i]) != 0) {
                        i++;
                    }*/
                    token = strtok(NULL ," \t\n");      /* get label name after .extern declaration */
                    if (token == NULL) {
                        isError = true;
                        printError(asFileName, INCOMPLETE_CODE, n);
                        memset(line,0,LINE_LENGTH);
                        continue;
                    }

                    int ind = 0;
                    if (strlen(token) >= LABEL_LEN) {
                        isError = true;
                        printError(asFileName, LABEL_LIMIT_REACHED, n);
                        memset(line,0,LINE_LENGTH);
                        continue;
                    }
                    if (isalpha(token[0]) ==0) {
                        isError = true;
                        printError(asFileName, BAD_LABEL_NAME, n);
                        memset(line,0,LINE_LENGTH);
                        continue;
                    }
                    /* label name is valid here */
                    strcpy(tempLabel.name, token);
                    tempLabel.address = 0;                  /* extern label */
                    tempLabel.value = 0;
                    tempLabel.offset = 0;
                    tempLabel.locationType = Extern;
                    tempLabel.dataType = NoneDataOrCode;

                    addDataNode(&labels, tempLabel);
                    memset(line,0,LINE_LENGTH);
                    continue;
                }
                isError = true;
                printError(asFileName, INCOMPLETE_CODE, n);
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
                Label tempLabel = {0};
                strcpy(tempLabel.name, tName);
                tempLabel.value = IC;
                tempLabel.address = (IC/16)*16;
                tempLabel.offset = IC - tempLabel.address;
                tempLabel.locationType = NoneExtOrEnt;
                tempLabel.dataType = Code;

                addDataNode(&labels, tempLabel);
                printf("labelName in tempLabel is: %s\n",tempLabel.name );
            }

            /* BUG: needs fixing */
            token = strtok(NULL, " \t");
            int opCode = getOpcode(token);

            if (opCode == -1) {
                isError = true;
                printError(asFileName, UNKNOWN_OPERATION, n);
                continue;
            }

            int funct = getFunct(opCode, token);

            Word opcodeWord;
            opcodeWord.code.opcode = opCode;
            opcodeWord.are = A;

            addWordNode(&words, opcodeWord, IC);
            IC++;

            if (opCode < 5) { /* 2 args */

            }
            else if (opCode < 14) { /* 1 arg */

            }
            memset(line,'\0',LINE_LENGTH);
            clearLine(line);
        }

        /* Need to save a final version of IC and DC here, idk where though */

        free(asFileName);

        return isError;
    }

    /*
    A function to check if the first word in the line is a label type, meaning:
        - it starts with an alpha
        - it's all alpha/num
    */
    int isLabelCheck(char* line) {
        int flag ,len; /* We will use this to set what situation we're in */
        char label[31] = {0};
        len = 0; flag = 0;
        for (i = 0; line[i] != '\n'; ++i) {
            if (isSpace(line[i]) != 0) {
                if (flag != 0)
                    return 1;
                else
                    continue;
            }

            if (line[i] == ':' && flag == 1)
                break;

            if (flag == 0) {
                if (isalpha(line[i]) != 0) {
                    flag++;
                    label[len] = line[i];
                    len++;
                    continue;
                }
                else
                    return 1;
            }

            if (flag == 1) {
                if (isalnum(line[i])) {
                    len++;
                    if (len > 31) {
                        return 2; /* ERROR: Label name too long! */
                    }
                    else
                        label[len-1] = line[i];
                }
                else
                    return 1;
            }

        }

        label[len] = '\0';
        if (getOpcode(label) != -1) {
            return 3; /* ERROR: Label can't be named that! */
        }

        if (contains(labels, label) != -1) {
            return 4; /* ERROR: Label already exists */
        }

        strcpy(tName, label);
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
            if (strcmp(operation, "add")) return 10;
            if (strcmp(operation, "sub")) return 10;
            break;
        case 5:
            if (strcmp(operation, "clr")) return 10;
            if (strcmp(operation, "not")) return 11;
            if (strcmp(operation, "inc")) return 12;
            if (strcmp(operation, "dec")) return 13;
            break;
        case 9:
            if (strcmp(operation, "jmp")) return 10;
            if (strcmp(operation, "bne")) return 11;
            if (strcmp(operation, "jsr")) return 12;
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