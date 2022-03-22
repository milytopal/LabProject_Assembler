#ifndef DATA_LINKED_LIST_H
#define DATA_LINKED_LIST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataStructs.h"

void addLabelNode(const char* name, int value, eDataType dataType, eLocalizaion locationType);/* Constructs a label node and adds it to list
                                    * the constructor calculates the address and offset inside
                                    * Input: name, value (the IC+DC counters), data/code type and localization type */

void deleteLabelList(pLabelNode list); /* delete the given list */

bool wasTheLabelDeclared(char*);   /* checks if a label name is already exsits in the list
                                    * Input: label name
                                    * Output: true is the label exists false if not */

pLabelNode duplicateExternLabelNode(pLabelNode prevLabel, pLabelNode nextLabel, int address); /* for the use of the externals file
                                    * every use of an external label encountered creates a copy of a given label and inserts it near the previous occurrences
                                    * Input: the to labels for the new one to be inserted between and the address for the base and offset
                                    * Output: the new label node created */


#endif
