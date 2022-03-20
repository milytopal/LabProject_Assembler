
#include "dataLinkedList.h"

void addLabelNode(const char* name, const int value, eDataType dataType, eLocalizaion locationType) {

    pLabelNode curr = NULL;
    if (labelsHead == NULL) {
        labelsHead = (pLabelNode) calloc(1, sizeof(LabelNode));
        labelsHead->label.value = value;
        labelsHead->label.offset = (value % 16);
        labelsHead->label.address = (value - (value % 16));
        labelsHead->label.dataType = dataType;
        labelsHead->label.locationType = locationType;
        strncpy(labelsHead->label.name, name, strlen(name));
        labelsHead->pNext = NULL;
        return;
    } else {
        curr = labelsHead;
        while (curr->pNext != NULL) {
            curr = curr->pNext;
        }
        curr->pNext = (pLabelNode) calloc(1, sizeof(LabelNode));
        curr = curr->pNext;
        curr->label.value = value;
        curr->label.offset = (value % 16);
        curr->label.address = (value - (value % 16));
        curr->label.dataType = dataType;
        curr->label.locationType = locationType;
        strncpy(curr->label.name, name, strlen(name));
        curr->pNext = NULL;
    }
}

void deleteLabelList(pLabelNode list)
{

    pLabelNode nextNode = NULL;
    while(list != NULL)
    {
        nextNode = list->pNext;
        free(list);
        list = nextNode;
    }
    list = NULL;
}

int contains(pLabelNode list, char* contain)
{
    int ind;
    pLabelNode lp;
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



