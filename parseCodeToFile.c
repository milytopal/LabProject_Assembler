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

    strcpy(codeFileName, fileName);
    strcat(codeFileName, ".ob");
    strcpy(extFileName, fileName);
    strcat(extFileName, ".ext");
    strcpy(entFileName, fileName);
    strcat(entFileName, ".ent");

    if (getcwd(cwd, sizeof(cwd)) != NULL) {     /*todo : remove later*/
        printf("Current working dir: %s\n", cwd);
    } else {
        perror("getcwd() error");
    }                                           /*todo : remove later*/

    codeF = fopen(codeFileName, "w");        /* create the .obj file */

    if (codeF == NULL){
        printError(codeFileName, FAILED_TO_CREATE, 0);
        printf("couldnt open file: %s\n ",codeFileName);            /* todo:  remove line */
        return true;
    }else
    {
        pWordNode t = (pWordNode) malloc(sizeof(pWordNode));
        t->word.address = 100;
        t->word.are = A;
        t->word.code.operands.destAdd = 0x3;
        t->word.code.operands.destReg = 0x4;
        t->word.code.operands.funct = 0x3;
        wordsHead = t;
        pWordNode t2 = (pWordNode) malloc(sizeof(WordNode));
        t2->word.address = 101;
        t2->word.are = R;
        t2->word.code.operands.destAdd = 0x2;
        t2->word.code.operands.destReg = 0x7;
        t2->word.code.operands.funct = 0x9;
        t2->word.code.operands.srcAdd = 0x1;
        t2->word.code.operands.srcReg = 0x3;

        wordsHead->pNext = t2;

        pWordNode t3 = (pWordNode) malloc(sizeof(WordNode));
        t3->word.address = 101;
        t3->word.are = E;
        t3->word.code.operands.destAdd = 0x0;
        t3->word.code.operands.destReg = 0x8;
        t3->word.code.operands.funct = 0x2;
        t3->word.code.operands.srcAdd = 0x1;
        t3->word.code.operands.srcReg = 0x3;

        wordsHead->pNext->pNext = t3;

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
    char* line = NULL;
    if(labelsHead == NULL)
        return;
    line = (char*)calloc(LINE_LENGTH,sizeof(char));




}
void printEntriesFile(FILE *fp)
{


}
void printObjectFile(FILE *fp)
{
    pWordNode curr = wordsHead;
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
    line = (char*)calloc(30,sizeof(char));
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
        c = ('a' + (n-10));
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
