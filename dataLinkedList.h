#ifndef DATA_LINKED_LIST_H
#define DATA_LINKED_LIST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataStructs.h"

void addDataNode(const char* name, int value, eDataType dataType, eLocalizaion locationType);
void deleteDataList(pLabelNode);

int contains(pLabelNode, char*);


#endif
