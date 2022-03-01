#ifndef DATA_LINKED_LIST_H
#define DATA_LINKED_LIST_H
#include "dataStructs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void addDataNode(pDataNode, pDataNode);
void removeDataNode(pDataNode, int);

pDataNode getDataNode(pDataNode, int);
int lengthDataNode(pDataNode);
int contains(pDataNode, DataNode);


#endif //DATA_LINKED_LIST_H
