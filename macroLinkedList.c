
#include "macroLinkedList.h"

int lengthMacroNode(pMacroNode list) {
    int i = 0;
    pMacroNode lp;
    for (lp = list; lp != NULL; lp = lp->pNext) {
        i++;
    }
    return i;
}

pMacroNode getMacroNode(pMacroNode list, int index){

    pMacroNode lp = list;
    int i;
    for (i = 0; i < index; ++i) {
        lp = lp->pNext;
    }
    if (lp == NULL) {
        printf("[WARNING]: get# on MacroNode, index out of bounds! Returned NULL! (%d / %d)", index, lengthMacroNode(list));
    }
    return lp;
}

void addMacroNode(pMacroNode list, pMacroNode toAdd){
    getMacroNode(list, lengthMacroNode(list))->pNext = toAdd;
}

void removeMacroNode(pMacroNode list, int index) {
    pMacroNode node = getMacroNode(list, index-1);
    node->pNext = node->pNext->pNext;
}
