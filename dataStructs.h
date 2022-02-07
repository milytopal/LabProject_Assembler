//
// Created by Mily Topal on 06/02/2022.
//

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

#define STARTING_IC 100
typedef enum bool{faalse,true}bool;
typedef enum eLocalizaion{Extern, Entry, NoneExtOrEnt}eLocalizaion;
typedef enum eDataType{String, Data, NoneDataOrStr}eDataType;
typedef enum ARE{A=4,R=2,E=1}ARE ;
enum eCommands{MOV,CMP,ADD,SUB,LEA,CLR,NOT,INC,DEC,JMP,BNE,JSR,RED,PRN,RTS,STOP};
enum eDirectives{DATA,STRING,ENTRY,EXTERN};

typedef struct Label{
    char label[LABEL_LEN];
    unsigned int address;
    eLocalizaion locationType;
    eDataType dataType;
}Label;

typedef struct DataNode *pDataNode;
typedef struct DataNode{
    int address;

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
    pWordNode pNextWord;
}WordNode;





#endif //DATASTRUCTS_H
