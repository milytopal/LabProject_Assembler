
#include "dataLinkedList.h"

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
            printf(" %s ", curr->label.name);
            curr = curr->pNext;
        }
        curr->pNext = (pDataNode) calloc(1, sizeof(DataNode));
        curr->pNext->label = label;
        curr->pNext->pNext = NULL;

    }
}

void deleteDataList(pDataNode list)
{
    pDataNode nextNode = NULL;
    while(labelsHead != NULL)
    {
        nextNode = labelsHead->pNext;
        free(labelsHead);
        labelsHead = nextNode;
    }
    labelsHead = NULL;
}

int contains(pDataNode list, char* contain)
{
    int ind;
    pDataNode lp;
    lp = labelsHead;
    ind = 0;
    while (lp != NULL) {
        if (strcmp(lp->label.name, contain) == 0) {
            return ind;
        }
        lp = lp->pNext;
        ind++;
    }

    return -1;
}



