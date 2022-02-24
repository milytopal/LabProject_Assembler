
#include "wordLinkedList.h"

int lengthWordNode(pWordNode list) {
    int i = 0;
    pWordNode lp;
    for (lp = list; lp != NULL; lp = lp->pNext) {
        i++;
    }
    return i;
}

pWordNode getWordNode(pWordNode list, int index){

    pWordNode lp = list;
    int i;
    for (i = 0; i < index; ++i) {
        lp = lp->pNext;
    }
    if (lp == NULL) {
        printf("[WARNING]: get# on WordNode, index out of bounds! Returned NULL! (%d / %d)", index, lengthWordNode(list));
    }
    return lp;
}

void addWordNode(pWordNode list, pWordNode toAdd){
    pWordNode temp = getWordNode(list, lengthWordNode(list))->pNext = toAdd;
    temp = (pWordNode) calloc(sizeof(WordNode), 1);
    temp = toAdd;
}

void removeWordNode(pWordNode list, int index) {
    pWordNode node = getWordNode(list, index-1);
    node->pNext = node->pNext->pNext;
}
