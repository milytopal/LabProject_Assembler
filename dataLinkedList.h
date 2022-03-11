#ifndef DATA_LINKED_LIST_H
#define DATA_LINKED_LIST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataStructs.h"

void addDataNode(Label label);
void deleteDataList(pDataNode, int);

pDataNode getDataNode(pDataNode, int);
int lengthDataNode(pDataNode);
int contains(pDataNode, char*);


#endif
