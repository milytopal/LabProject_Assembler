
#include "firstPass.h"


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

    int IC = STARTING_IC;
    int DC = 0;
    bool isError = false;

    int n = 0;
    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), fp) != NULL) {
        n++;

        /* Sould probably check if it's an empty line here */

        if (&line[MAX_LINE_LEN-1] != NULL && line[MAX_LINE_LEN-1] != '\0') {
            printError(asFileName, LINE_LIMIT_REACHED, n);
            return true; /* Can't actually check the line */
        }

        /* isLabelCheck */

         /* .data/.string? */

         /* .extern/.entry? Don't continue if so */

         /* everything else */
    }

    /* Need to save a final version of IC and DC here, idk where though */

    free(asFileName);

    return isError;
}

/*
A function to check if the first word in the line is a label type, meaning:
    - it starts with a 
*/
static bool isLabelCheck(char* line) {





    return true;
}
