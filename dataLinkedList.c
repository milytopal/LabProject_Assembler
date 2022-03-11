
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

void addDataNode(const Label label) {

    pDataNode curr = NULL;
    if (labelsHead == NULL) {
        labelsHead = (pDataNode) calloc(1, sizeof(DataNode));
        labelsHead->label = label;
        labelsHead->pNext = NULL;
        printf("%s", labelsHead->label.name);
        return;
    } else {
        curr = labelsHead;
        while (curr->pNext != NULL) {
            printf("\n %s ", curr->label.name);
            curr = curr->pNext;
        }
        curr->pNext = (pDataNode) calloc(1, sizeof(DataNode));
        curr->pNext->label = label;
        curr->pNext->pNext = NULL;

    }
}



void deleteDataList(pDataNode list, int index) {
    pDataNode node = getDataNode(list, index-1);
    node->pNext = node->pNext->pNext;
}

int contains(pDataNode list, char* contain) {
    
    pDataNode lp = NULL;
    lp = labelsHead;
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



