#ifndef WORD_LINKED_LIST_H
#define WORD_LINKED_LIST_H
#include "dataStructs.h"
#include <stdio.h>
#include <stdlib.h>


/*void addWordNode(pWordNode, pWordNode);*/
void addWordNode(pWordNode,Word,int);
void removeWordNode(pWordNode, int);

pWordNode getWordNode(pWordNode, int);
int lengthWordNode(pWordNode);


#endif 