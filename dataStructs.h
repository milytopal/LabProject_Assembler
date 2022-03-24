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
#define MAX_RAM_ADDRESS 8191

#define LINE_LENGTH 83
#define LABEL_DEST_B 'B'    /*marking what data is missing*/
#define LABEL_DEST_O 'O'    /*marking what data is missing*/
#define STARTING_IC 100
#define NUM_OF_REGS 16

#define NO_ARGS_EXPECTED 0
#define EXPECTED_ONE_ARG 1
#define EXPECTED_TWO_ARGS 2

typedef enum bool{false,true}bool;

typedef enum eLocalizaion{Extern, Entry, NoneExtOrEnt }eLocalizaion;

typedef enum eDataType{Data, Code, NoneDataOrCode }eDataType;

typedef enum ARE{A=4,R=2,E=1 }ARE ;

typedef enum eDirectives{DATA,STRING,ENTRY,EXTERN }eDirectives;

typedef enum eAddrresMethod{ NA = -1, IMMEDIATE=0,DIRECT,INDEX,DIRECT_REGISTER }eAddrresMethod;

typedef enum eErrorCode { FAILED_TO_OPEN = 0 ,MISSING_FILE,LINE_LIMIT_REACHED,
    LABEL_LIMIT_REACHED, BAD_LABEL_NAME, FAILED_TO_CREATE,
    NO_ARGUMENTS, INCOMPLETE_CODE,MISSING_LABEL, MISSING_PARAMETER, EXTRA_TEXT_AT_END,
    LABEL_ALREADY_EXISTS, UNKNOWN_OPERATION, INVALID_ARGUMENT,
    NUMBER_OUT_OF_BOUND, INVALID_BRACKET_CONTENTS , TOO_MANY_ARGUMENTS,
    INVALID_USE_OF_REGISTER , MISSING_BRACKETS ,
    LABEL_IS_ALREADY_EXTERN, LABEL_DOSNT_EXIST,MISSING_ARGUMENTS,
    LABEL_WASNT_USED_AS_ENTRY, USE_OF_SAVED_WORD, MAX_RAM_EXCITED,
    EXTRA_COMMA_AT_END }eErrorCode;

/* in each opcode only one bit is set to '1' */
typedef enum eCommands{
    MOV = 1,            /* 2^0 = 1*/
    CMP = 2,            /* 2^1 = 2*/
    ADD = 4,            /* 2^2 = 4*/
    SUB = 4,            /* 2^2 = 4*/
    LEA = 16,           /* 2^4 = 16*/
    CLR = 32,           /* 2^5 = 32*/
    NOT = 32,           /* 2^5 = 32*/
    INC = 32,           /* 2^5 = 32*/
    DEC = 32,           /* 2^5 = 32*/
    JMP = 512,          /* 2^9 = 512*/
    BNE = 512,          /* 2^9 = 512*/
    JSR = 512,          /* 2^9 = 512*/
    RED = 4096,         /* 2^12 = 4096*/
    PRN = 8192,         /* 2^13 = 8192*/
    RTS = 16384,        /* 2^14 = 16384*/
    STOP = 32768        /* 2^15 = 32768*/
}eCommands;

/* the TokenNode is a node for linked list of commands inside a macro */
typedef struct TokenNode *pTokenNode;
typedef struct TokenNode{
    char token[LINE_LENGTH];
    pTokenNode pNext;
} TokenNode;

/* Macro wasTheLabelDeclared a name and a linked list of commands */
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

/* three lists designed as single occurrence meaning there is only one label list in the code
 * every class in project includes the data structs header file there for the lists does not nned to be declared as externs*/
pWordNode wordsHead;
pLabelNode labelsHead;
pWordNode datasHead;

int IC;
int DC;

#endif
