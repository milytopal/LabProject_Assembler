#ifndef PARSE_CODE_TO_FILE_H
#define PARSE_CODE_TO_FILE_H
#include "dataStructs.h"
#include "utils.h"

bool parseCodeToFile(char *fileName, bool error);

/* convert unsigned int to ascii*/
char uitoa(int n);
char *printByte(int index, int value);
char *parseWordToBase(pWordNode word);
char *ParseExternals(pLabelNode);

void printExternalsFile(FILE *fp);
void printEntriesFile(FILE *fp);
void printObjectFile(FILE *fp);


#endif
