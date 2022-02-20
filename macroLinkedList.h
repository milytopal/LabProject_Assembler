#ifndef MACRO_LINKED_LIST_H
#define MACRO_LINKED_LIST_H
#include "dataStructs.h"
#include <stdio.h>



void addMacroNode(pMacroNode, pMacroNode);
void removeMacroNode(pMacroNode, int);

pMacroNode getMacroNode(pMacroNode, int);
int lengthMacroNode(pMacroNode);


#endif //MACRO_LINKED_LIST_H