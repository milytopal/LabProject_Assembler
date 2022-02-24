#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

#define LABEL_LEN 31
#define OPCODE_LEN 6
#define SRC_ADD_LEN 2
#define SRC_REG_LEN 3
#define DEST_ADD_LEN 2
#define DEST_REG_LEN 3
#define FUNCT_LEN 5
#define ARE_FIELD_LEN 3

#define LINE_LENGTH 83

//#define NULL ((void *)0)

#define STARTING_IC 100
#define NUM_OF_CMD 16
#define MACRO_STR "macro"

typedef enum bool{false,true}bool;
typedef enum eLocalizaion{Extern, Entry, NoneExtOrEnt}eLocalizaion;
typedef enum eDataType{String, Data, NoneDataOrStr}eDataType;
typedef enum ARE{A=4,R=2,E=1}ARE ;
typedef enum eCommands{MOV = 0,CMP,ADD,SUB,LEA,CLR,NOT,INC,DEC,JMP,BNE,JSR,RED,PRN,RTS,STOP}eCommands;
typedef enum eDirectives{DATA,STRING,ENTRY,EXTERN}eDirectives;
typedef enum eErrorCode{MISSING_FILE_NAME,MISSING_FILE}eErrorCode;
/* array to hold command strings for comparison*/
//char cmdStr


typedef struct TokenNode *pTokenNode;
typedef struct TokenNode{
    char token[LINE_LENGTH];
    pTokenNode pNext;
} TokenNode;

typedef struct Macro{
    char name[LABEL_LEN];
    TokenNode* tokenList;
}Macro;

typedef struct MacroNode *pMacroNode;
typedef struct MacroNode{
    Macro macro;
    pMacroNode pNext;
} MacroNode;

typedef struct Label{
    char label[LABEL_LEN];
    unsigned int address;
    eLocalizaion locationType;
    eDataType dataType;
}Label;

typedef struct DataNode *pDataNode;
typedef struct DataNode{
    int address;
    char *data;
    pDataNode pNext;
}DataNode;

typedef struct Word{
    unsigned long index;
    union {
        unsigned int ARE:ARE_FIELD_LEN;
        unsigned int funct:FUNCT_LEN;
        unsigned int destReg:DEST_REG_LEN;
        unsigned int destAdd:DEST_ADD_LEN;
        unsigned int srcReg:SRC_REG_LEN;
        unsigned int srcAdd:SRC_ADD_LEN;
        unsigned int opcode:OPCODE_LEN;
    } code;
}Word;

/* represents every line in the code */
typedef struct WordNode* pWordNode;
typedef struct WordNode{
    unsigned int address;
    Word word;
    pWordNode pNext;
}WordNode;





#endif //DATASTRUCTS_H
