#include "firstPass.h"

int i;
char* tName;

bool firstPass(const char* fileName, bool firstPass)
{
    FILE *fp = NULL;
    char* asFileName;
    asFileName = (char*)calloc(strlen(fileName) + strlen(".am") + 1, sizeof(char));
    strcat(asFileName, fileName);
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
    while (fgets(line, sizeof(line), fp) != NULL) {
        n++;
        i = 0;
        isLabel = false;

        if (isEmptyLine(line) == true) continue;

        if (strlen(line) > LINE_LENGTH) {
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

         if (line[i] == '.') {
             /* Handle .string .data .entry .extern */
            
            int isData = strcmp(line+i, ".data");
            int isString = strcmp(line+i, ".string");
            if (isData == 0 || isString == 0) {
                if (isLabel == true) {
                    Label tempLabel;
                    strcpy(tempLabel.name, tName);
                    tempLabel.value = IC;
                    tempLabel.address = (IC/16)*16;
                    tempLabel.offset = IC - tempLabel.address;
                    tempLabel.dataType = Data;
                    tempLabel.locationType = NoneExtOrEnt;

                    pDataNode tempData = (pDataNode) calloc(sizeof(DataNode), 1);
                    tempData->label = tempLabel;
                    tempData->pNext = NULL;
                    addDataNode(labels, tempData);
                }
                while (isSpace(line[i]) != 0) {
                    i++;
                }

                if (line[i] == '\n') {
                    isError = true;
                    printError(asFileName, NO_ARGUMENTS, n);
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
                        tempWord.code.are = A;

                        pWordNode tempWordNode = (pWordNode) calloc(sizeof(pWordNode), 1);
                        tempWordNode->word = tempWord;
                        tempWordNode->address = IC+DC;
                        tempWordNode->pNext = NULL;
                        
                        addWordNode(words, tempWordNode);
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
                        tempWord.code.are = A;

                        pWordNode tempWordNode = (pWordNode) calloc(sizeof(pWordNode), 1);
                        tempWordNode->word = tempWord;
                        tempWordNode->address = IC+DC;
                        tempWordNode->pNext = NULL;

                        addWordNode(words, tempWordNode);
                        DC++;
                    }
                    /* Now we need to add '\0' to the string */
                    Word tempWord;
                    tempWord.code.opcode = 0;
                    tempWord.code.are = A;

                    pWordNode tempWordNode = (pWordNode) calloc(sizeof(pWordNode), 1);
                    tempWordNode->word = tempWord;
                    tempWordNode->address = IC+DC;
                    tempWordNode->pNext = NULL;

                    addWordNode(words, tempWordNode);
                    DC++;
                }
                continue;
            }
            else if (strcmp(line+i, ".entry") == 0) {
                /* Handled on secondPass ! ! */
                continue;
            }
            else if (strcmp(line+i, ".extern") == 0) {
                i += 8;
                Label tempLabel;
                char name[31];
                while (isSpace(line[i]) != 0) {
                    i++;
                }

                if (line[i] == '\n') {
                    isError = true;
                    printError(asFileName, INCOMPLETE_CODE, n);
                    continue;
                }

                int ind = 0;
                if (isalpha(line[i])) {
                    while (isSpace(line[i]) == 0) {
                        name[ind++] = line[i++];
                    }
                }
                else {
                    isError = true;
                    printError(asFileName, BAD_LABEL_NAME, n);
                    continue;
                }

                if (ind >= 31) {
                    isError = true;
                    printError(asFileName, LABEL_LIMIT_REACHED, n);
                    continue;
                }

                strcmp(tempLabel.name, name);
                tempLabel.address = 0;
                tempLabel.value = 0;
                tempLabel.offset = 0;
                tempLabel.locationType = Extern;
                tempLabel.dataType = NoneDataOrCode;

                pDataNode tempData = (pDataNode) calloc(sizeof(DataNode), 1);
                tempData->label = tempLabel;
                tempData->pNext = NULL;
                addDataNode(labels, tempData);
                
                continue;
            }
            isError = true;
            printError(asFileName, INCOMPLETE_CODE, n);
            continue;
         } /* Ending of handling .string .data .entry .extern */

         /* everything else */
        if (isLabel == false) {
            i = 0;
            while (isSpace(line[i]) != 0) {
                i++;
            }
        } /* Checked too much, it's prefered we start over with the line */
        else {
            Label tempLabel;
            strcmp(tempLabel.name, tName);
            tempLabel.value = IC;
            tempLabel.address = (IC/16)*16;
            tempLabel.offset = IC - tempLabel.address;
            tempLabel.locationType = NoneExtOrEnt;
            tempLabel.dataType = Code;

            pDataNode tempData = (pDataNode) calloc(sizeof(DataNode), 1);
            tempData->label = tempLabel;
            tempData->pNext = NULL;
            addDataNode(labels, tempData);
        }
        
        char *operation = strtok(line[i], ' ');
        int opCode = getOpcode(operation);

        if (opCode == -1) {
            isError = true;
            printError(asFileName, UNKNOWN_OPERATION, n);
            continue;
        }

        int funct = getFunct(opCode, operation);

        Word opcodeWord;
        opcodeWord.code.opcode = opCode;
        opcodeWord.code.are = A;

        pWordNode opcodeWordNode = (pWordNode) calloc(sizeof(pWordNode), 1);
        opcodeWordNode->word = opcodeWord;
        opcodeWordNode->address = IC;
        opcodeWordNode->pNext = NULL;
        addWordNode(words, opcodeWordNode);
        IC++;

        if (opCode < 5) { /* 2 args */

        }
        else if (opCode < 14) { /* 1 arg */

        }


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
    int flag = 0; /* We will use this to set what situation we're in */
    char label[31];
    int len = 0;
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