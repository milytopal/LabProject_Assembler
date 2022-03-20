#include <stdio.h>

#include "dataStructs.h"
#include "utils.h"
#include "preProcessor.h"
#include "firstPass.h"
#include "wordLinkedList.h"
#include "dataLinkedList.h"
#include "secondPass.h"
#include "parseCodeToFile.h"

bool isErrorFound; /* global variable - represents if error found in current file*/

int main(int argc, char *argv[]) {
    int i;
    int ICF,DCF;
    ICF = 0;
    DCF = 0;
    isErrorFound = false;
    if(argc < 1)
    {
        fprintf(stderr, "Missing Arguments for Assembler \n ");
    }

/* todo: check if assembler is another argument or it is the name of the program */
   /* if(strcmp(argv[1],"assembler") != 0)
    {
        fprintf(stderr, "Invalid Operation! %s is Unknown Process \n",argv[1]);
    }*/
    for(i=1; i < argc ; i++)
    {
        /* error will occur only if file is missing or failed to open file */
        isErrorFound = PreProcessPass(argv[i]);

        if(isErrorFound == false)
        {
            isErrorFound = firstPass(argv[i],&ICF, &DCF);
        }
        if(isErrorFound == false)
        {
        fprintf(stderr, " befpre <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< secondPass \n ");
            isErrorFound = secondPass(argv[i], &ICF,&DCF);
	fprintf(stderr, " after <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< secondPass \n ");        
	}
        if(isErrorFound == false)
        {
 	fprintf(stderr, " before <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< parseCodeToFile \n ");
            isErrorFound = parseCodeToFile(argv[i], &ICF,&DCF);
	 fprintf(stderr, " after <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< parseCodeToFile \n ");        
	}
        /* todo: reset linked lists and everything before next file*/
        deleteWordList(wordsHead);
fprintf(stderr, "deleteWordList(wordsHead) in main\n ");
        deleteWordList(datasHead);
fprintf(stderr, "deleteWordList(datasHead) in main \n ");
        deleteLabelList(labelsHead);
fprintf(stderr, "deleteLabelList(datasHead) in main \n ");

    }

    return 0;
}
