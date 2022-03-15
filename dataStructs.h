#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LABEL_LEN 31
#define OPCODE_LEN 16
#define SRC_ADD_LEN 2
#define SRC_REG_LEN 4
#define DEST_ADD_LEN 2
#define DEST_REG_LEN 4
#define FUNCT_LEN 4
#define ARE_FIELD_LEN 4

#define LINE_LENGTH 83
#define LABEL_DEST_B 'B'    /*marking what data is missing*/
#define LABEL_DEST_O 'O'    /*marking what data is missing*/
#define STARTING_IC 100
#define NUM_OF_REGS 16

typedef enum bool{false,true}bool;
typedef enum eLocalizaion{Extern, Entry, NoneExtOrEnt}eLocalizaion;
typedef enum eDataType{Data, Code, NoneDataOrCode}eDataType;
typedef enum ARE{A=4,R=2,E=1}ARE ;
typedef enum eCommands{MOV,CMP,ADD,SUB,LEA,CLR,NOT,INC,DEC,JMP,BNE,JSR,RED,PRN,RTS,STOP}eCommands;
typedef enum eDirectives{DATA,STRING,ENTRY,EXTERN}eDirectives;
typedef enum eAddrresMethod{IMMEDIATE=0,DIRECT,INDEX,DIRECT_REGISTER}eAddrresMethod;
typedef enum eErrorCode { MISSING_FILE_NAME,MISSING_FILE,LINE_LIMIT_REACHED, LABEL_LIMIT_REACHED, BAD_LABEL_NAME, FAILED_TO_CREATE,
    NO_ARGUMENTS, INCOMPLETE_CODE,MISSING_LABEL, MISSING_PARAMETER,
    LABEL_ALREADY_EXISTS, UNKNOWN_OPERATION, INVALID_ARGUMENT,
    NUMBER_OUT_OF_BOUND, INVALID_BRACKET_CONTENTS , TOO_MANY_ARGUMENTS,
    INVALID_USE_OF_REGISTER , MISSING_BRACKETS ,
    LABEL_IS_ALREADY_EXTERN, LABEL_DOSNT_EXIST}eErrorCode;

/* the TokenNode is a node for linked list of commands inside a macro */
typedef struct TokenNode *pTokenNode;
typedef struct TokenNode{
    char token[LINE_LENGTH];
    pTokenNode pNext;
} TokenNode;

/* Macro contains a name and a linked list of commands */
typedef struct Macro{
    char name[LABEL_LEN];
    TokenNode* tokenList;
}Macro;

/* MacroNode is a struct for a linked list of every macro declaration in the source file*/
typedef struct MacroNode *pMacroNode;
typedef struct MacroNode{
    Macro macro;
    pMacroNode pNext;
} MacroNode;

typedef struct Label{
    char name[LABEL_LEN];
    unsigned int address;
    int value;
    int offset;
    eLocalizaion locationType;
    eDataType dataType;
}Label;

typedef struct LabelNode *pLabelNode;
typedef struct LabelNode{
    Label label;
    pLabelNode pNext;
}LabelNode;

typedef struct twoOperands
{
    unsigned int destAdd:DEST_ADD_LEN;
    unsigned int destReg:DEST_REG_LEN;
    unsigned int srcAdd:SRC_ADD_LEN;
    unsigned int srcReg:SRC_REG_LEN;
    unsigned int funct:FUNCT_LEN;
}twoOperands;

/* Word structure:          |0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|
*                           |0|A|R|E|          OPCODE               |
*                           |0|A|R|E| FUNCT | SRCREG|<AM|DESTREG|<AM|
* AM = ADDRESSING METHOD: 0 = IMMEDIATE ,1 = DIRECT , 2 = INDEX , 3 = REGISTER DIRECT */
typedef struct Word{
    union {
        twoOperands operands;
        unsigned int opcode:OPCODE_LEN;
    } code;
    unsigned int are:ARE_FIELD_LEN;
    unsigned int address;
    char name[LABEL_LEN];
    char labelDest;
    int lineNum;
    bool isLabel;
}Word;


/* represents every line in the code */
typedef struct WordNode* pWordNode;
typedef struct WordNode{
    Word word;
    pWordNode pNext;
}WordNode;

/* three lists designed as single occurrence
 * meaning there cannot be another label list in the code*/
pWordNode wordsHead;
pLabelNode labelsHead;
pWordNode datasHead;


/*pLabelNode labelsHead = NULL;
pWordNode codeHead = NULL; */

int IC;
int DC;

#endif
