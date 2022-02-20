
#include "dataLinkedList.h"

int lengthDataNode(pDataNode list) {
    int i = 0;
    pDataNode lp;
    for (lp = list; lp != NULL; lp = lp->pNext) {
        i++;
    }
    return i;
}

pDataNode getDataNode(pDataNode list, int index){

    pDataNode lp = list;
    int i;
    for (i = 0; i < index; ++i) {
        lp = lp->pNext;
    }
    if (lp == NULL) {
        printf("[WARNING]: get# on DataNode, index out of bounds! Returned NULL! (%d / %d)", index, lengthDataNode(list));
    }
    return lp;
}

void addDataNode(pDataNode list, pDataNode toAdd){
    getDataNode(list, lengthDataNode(list))->pNext = toAdd;
}

void removeDataNode(pDataNode list, int index) {
    pDataNode node = getDataNode(list, index-1);
    node->pNext = node->pNext->pNext;
}
