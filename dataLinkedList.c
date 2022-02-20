
#include "dataLinkedList.h"

int length(pDataNode list) {
    int i = 0;
    pDataNode lp;
    for (lp = list; lp != NULL; lp = lp->pNext) {
        i++;
    }
    return i;
}

pDataNode get(pDataNode list, int index){

    pDataNode lp = list;
    int i;
    for (i = 0; i < index; ++i) {
        lp = lp->pNext;
    }
    if (lp == NULL) {
        printf("[WARNING]: get# on DataNode, index out of bounds! Returned NULL! (%d / %d)", index, length(list));
    }
    return lp;
}

void add(pDataNode list, pDataNode toAdd){
    get(list, length(list))->pNext = toAdd;
}

void remove(pDataNode list, int index) {
    pDataNode node = get(list, index-1);
    node->pNext = node->pNext->pNext;
}
