
#include "firstPass.h"

int i;

bool firstPass(const char* fileName, bool firstPass)
{
    FILE *fp = NULL;
    char* asFileName;
    asFileName = (char*)calloc(strlen(fileName) + strlen(".as") + 1, sizeof(char));
    strcat(asFileName, fileName);
    strcat(asFileName, ".as");
    fp = fopen(asFileName, "r");

    if (fp == NULL){
        printError(NULL, MISSING_FILE, 0);
        return true;
    }

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

        /* Sould probably check if it's an empty line here */

        if (&line[LINE_LENGTH+1] != NULL && line[LINE_LENGTH+1] != '\n') {
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
                break;
            case 3:
                isError = true;
                printError(asFileName, BAD_LABEL_NAME, n);
                break;
        }

         while (isspace(line[i]) == 0) {
             i++;
         }

         if (line[i] == '.') {

         }

         /* everything else */
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
static int isLabelCheck(char* line) {
    int flag = 0; /* We will use this to set what situation we're in */
    char label[31];
    int len = 0;
    for (i = 0; line[i] != '\0'; ++i) {

        if (isspace(line[i]) != 0) {
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

    return 0;
}

static int getOpcode(char* label) {
    if (strcmp(label, "mov") == 0) return 0;
    if (strcmp(label, "cmp") == 0) return 1;
    if (strcmp(label, "add") == 0) return 2;
    if (strcmp(label, "sub") == 0) return 2;
    if (strcmp(label, "lea") == 0) return 4;
    if (strcmp(label, "clr") == 0) return 5;
    if (strcmp(label, "not") == 0) return 5;
    if (strcmp(label, "inc") == 0) return 5;
    if (strcmp(label, "dec") == 0) return 5;
    if (strcmp(label, "jmp") == 0) return 9;
    if (strcmp(label, "bne") == 0) return 9;
    if (strcmp(label, "jsr") == 0) return 9;
    if (strcmp(label, "red") == 0) return 12;
    if (strcmp(label, "prn") == 0) return 13;
    if (strcmp(label, "rts") == 0) return 14;
    if (strcmp(label, "stop") == 0) return 15;
    return -1;
}
