#ifndef WORD_LINKED_LIST_H
#define WORD_LINKED_LIST_H
#include "dataStructs.h"
#include <stdio.h>
#include <stdlib.h>


/*void addWordNodeToCode(pWordNode, pWordNode);*/
void addWordNodeToCode(Word word, int val, char labelDest , int lineN);
void deleteWordList(pWordNode);
void addWordNodeToData(Word word, int val, char labelDest , int lineN);


#endif
