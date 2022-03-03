
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
    if (list == NULL) {
        list = toAdd;
        return;
    }
    pDataNode temp = getDataNode(list, lengthDataNode(list))->pNext;
    temp = (pDataNode) calloc(sizeof(DataNode), 1);
    temp = toAdd; /* Warns that temp isn't used, but it is?? */
}

void removeDataNode(pDataNode list, int index) {
    pDataNode node = getDataNode(list, index-1);
    node->pNext = node->pNext->pNext;
}

int contains(pDataNode list, DataNode contain) {
    
    pDataNode lp = list;
    int ind = 0;
    while (lp != NULL) {
        if (strcmp(lp->label.name, contain.label.name) == 0) {
            return ind;
        }
        lp = lp->pNext;
        ind++;
    }

    return -1;
}



