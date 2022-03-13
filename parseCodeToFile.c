#include <unistd.h>
#include "parseCodeToFile.h"

bool parseCodeToFile(char *fileName, bool error)
{
    FILE *entF = NULL;
    FILE *extF = NULL;
    FILE *codeF = NULL;
    char* codeFileName;
    char* extFileName;
    char* entFileName;
    char cwd[LINE_LENGTH];      /*todo : remove later*/
    if(error == true)
        return true;

    codeFileName = (char*)calloc(strlen(fileName) + strlen(".ob") + 1, sizeof(char));
    extFileName = (char*)calloc(strlen(fileName) + strlen(".ext") + 1, sizeof(char));
    entFileName = (char*)calloc(strlen(fileName) + strlen(".ext") + 1, sizeof(char));

    strncpy(codeFileName, fileName, strlen(fileName));
    strcat(codeFileName, ".ob");
    strncpy(extFileName, fileName, strlen(fileName));
    strcat(extFileName, ".ext");
    strncpy(entFileName, fileName, strlen(fileName));
    strcat(entFileName, ".ent");

    codeF = fopen(codeFileName, "w");        /* create the .obj file */

    if (codeF == NULL){
        printError(codeFileName, FAILED_TO_CREATE, 0);
        printf("couldnt open file: %s\n ",codeFileName);            /* todo:  remove line */
        return true;
    }else
    {

        printObjectFile(codeF);
        /* parseWordToBase(&testing); */
        /* todo: print to code file */
    }
    fclose(codeF);

    extF = fopen(extFileName, "w");        /* create the .ext file */

    if (extF == NULL){
        printError(extFileName, FAILED_TO_CREATE, 0);
        printf("couldnt open file: %s\n ",extFileName);     /* todo:  remove line */
        return true;
    }else
    {
        /* todo: print to externals file */
    }
    fclose(extF);

    entF = fopen(entFileName, "w");        /* create the .ent file */

    if (entF == NULL){
        printError(entFileName, FAILED_TO_CREATE, 0);
        printf("couldnt open file: %s\n ",entFileName);      /* todo:  remove line */
        return true;
    }else
    {
        /* todo: print to entry file */
    }
    fclose(entF);
    free(codeFileName);
    free(entFileName);
    free(extFileName);

    return false;
}
void printExternalsFile(FILE *fp)
{
    pDataNode curr = NULL;
    char *toPrint = NULL;
    curr = labelsHead;
    if(labelsHead == NULL)          /* no labels in file */
        return;

    while(curr !=NULL)
    {
        toPrint = ParseExternals(curr);
        fprintf(fp,"%s",toPrint);
        curr = curr->pNext;
    }
    free(toPrint);

}

char *ParseExternals(pDataNode node)
{
    char* line = NULL;
    line = (char*)calloc(LINE_LENGTH,sizeof(char));

}

void printEntriesFile(FILE *fp)
{


}
void printObjectFile(FILE *fp)
{
    pWordNode curr = NULL;
    curr = wordsHead;
    char* toPrint = NULL;
    while(curr !=NULL)
    {
        toPrint = parseWordToBase(curr);
        fprintf(fp,"%s",toPrint);
        curr = curr->pNext;
    }
    free(toPrint);
}
char *parseWordToBase(pWordNode word)
{
    pWordNode tmp = NULL;
    int index ,val, i, wValue,opcode;
    int segments[5] = {0};
    Word tmpWord = {0};
    char* line = NULL;
    line = (char*)calloc(LINE_LENGTH,sizeof(char));
    tmpWord = word->word;

    wValue = (tmpWord.are << 16) + tmpWord.code.opcode;

    sprintf(line,"\n%04d",word->word.address);
    strcat(line,"\t");                  /* adding tab to format */
    /* crate segments 4 of the binary value  */
    for(i=0; i<5; i++)
    {
        segments[i] = wValue & 0x0F;
        wValue >>= 4;
    }
    strcat(line, printByte(0,segments[4]));
    strcat(line, "-");
    strcat(line, printByte(1,segments[3]));
    strcat(line, "-");
    strcat(line, printByte(2,segments[2]));
    strcat(line, "-");
    strcat(line, printByte(3,segments[1]));
    strcat(line, "-");
    strcat(line, printByte(4,segments[0]));

    return line;


}
/* convert unsigned int to ascii in hex base*/
char uitoa(int n)
{
    char c ;
    if(n < 0 || n > 15 )
        return '0';
    if(n >= 0 && n < 10)
    {
        c = ('0'+ n);
    }
    if(n > 9 && n < 16 )
    {
        c = ('a' + (n-0xA));
    }
    return c;
}

char *printByte(int index, int value)
{
    char *byte = NULL;
    byte = (char*) calloc(3,sizeof(char));
    switch(index)
    {
        case 0:
        {
            byte[0] = 'A';
            break;
        }
        case 1:
        {
            byte[0] = 'B';
            break;
        }
        case 2:
        {
            byte[0] = 'C';
            break;
        }
        case 3:
        {
            byte[0] = 'D';
            break;
        }
        case 4:
        {
            byte[0] = 'E';
            break;
        }
        default:
        {
            printf(" invalid index \n");
            break;
        }
    }
    byte[1] = uitoa(value);
    return byte;
}
