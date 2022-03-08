
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

void addDataNode(pDataNode* list, Label label){

    pDataNode tempData = NULL;
    pDataNode* curr = NULL;
    tempData = (pDataNode) calloc(1, sizeof(DataNode));
    tempData->label = label;
    tempData->pNext = NULL;
    curr = list;
    if(*curr == NULL)
    {
        *list = tempData;
        printf("labelName in list is: %s\n",tempData->label.name );

        return;
    }
    /* go to end of list */
    while((*curr)->pNext != NULL)
    {
        (*curr) = (*curr)->pNext;
    }
    (*curr)->pNext = tempData;
    printf("labelName in list is: %s\n",(*curr)->pNext->label.name );
}

void removeDataNode(pDataNode list, int index) {
    pDataNode node = getDataNode(list, index-1);
    node->pNext = node->pNext->pNext;
}

int contains(pDataNode list, char* contain) {
    
    pDataNode lp = list;
    int ind = 0;
    while (lp != NULL) {
        if (strcmp(lp->label.name, contain) == 0) {
            return ind;
        }
        lp = lp->pNext;
        ind++;
    }

    return -1;
}



