#ifndef PARSE_CODE_TO_FILE_H
#define PARSE_CODE_TO_FILE_H
#include "dataStructs.h"
#include "utils.h"

bool parseCodeToFile(char *fileName, const int *ICF, const int *DCF);

/* convert unsigned int to ascii*/
char uitoa(int n);
char *printByte(int index, int value);
char *parseWordToBase(pWordNode word);
char *ParseExternals(pLabelNode);

void printExternalsFile(FILE *fp);
void printLabelTableToFiles(FILE *extF, FILE *entF);
void printObjectFile(FILE *fp,const int *ICF,const int *DCF);


#endif
