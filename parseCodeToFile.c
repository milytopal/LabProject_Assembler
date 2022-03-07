#include <unistd.h>
#include "parseCodeToFile.h"

typedef struct segmentedMemory
{
    unsigned int e:4;
    unsigned int d:4;
    unsigned int c:4;
    unsigned int b:4;
    unsigned int a:4;
}segmentedMemory;
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

    codeF = fopen(codeFileName, "r");        /* create the .obj file */

    if (codeF == NULL){
        printError(codeFileName, FAILED_TO_CREATE, 0);
        printf("couldnt open file: %s\n ",codeFileName);            /* todo:  remove line */
        return true;
    }else
    {
        WordNode testing = {0};
        parseWordToBase(&testing);
        /* todo: print to code file */
    }
    fclose(codeF);

    extF = fopen(extFileName, "r");        /* create the .ext file */

    if (extF == NULL){
        printError(extFileName, FAILED_TO_CREATE, 0);
        printf("couldnt open file: %s\n ",extFileName);     /* todo:  remove line */
        return true;
    }else
    {
        /* todo: print to externals file */
    }
    fclose(extF);

    entF = fopen(entFileName, "r");        /* create the .ent file */

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



}
void printEntriesFile(FILE *fp)
{


}
void printObjectFile(FILE *fp)
{

}
char *parseWordToBase(pWordNode word)
{
    pWordNode tmp = NULL;
    segmentedMemory temp;
    int index ,val, i, areVal,restOfWord;
    char buff[20] = {0};
    int segments[5];
    Word tmpWord = {0};
    char* line = NULL;
    line = (char*)calloc(30,sizeof(char));
    word->address = 100;
    tmpWord.are = A;
    //tmpWord.code.opcode = 0x3200;
    tmpWord.code.destAdd = 0x5;
    tmpWord.code.destReg = 0;
    tmpWord.code.funct = 0x3;
    printf("%d\n",sizeof(tmpWord.code));

    sprintf(line,"%04d",word->address);
    strcat(line,"\t");                  /* adding tab to format */
    areVal = tmpWord.are;
    restOfWord = tmpWord.code.opcode;
    /* crate segments 4 of the binary value  */
    memcpy(&buff , &(tmpWord) ,sizeof (tmpWord));

    for(i=0; i<5; i++)
    {
        segments[i] = temp.a;
    }








}
/* convert unsigned int to ascii in hex base*/
char *uitoa(int n)
{
    char* c = NULL;
    c = (char*) calloc(1,sizeof (char));
    if(n <= 0 || n > 15 )
        return NULL;
    if(n >= 0 && n < 10)
    {
        *c = ('0'+n);
    }
    if(n > 9 && n < 16 )
    {
        *c = ('a' + n);
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
    byte[1] = *uitoa(value);
    return byte;
}
