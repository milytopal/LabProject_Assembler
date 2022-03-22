
#ifndef SECOND_PASS_H
#define SECOND_PASS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "dataLinkedList.h"
#include "dataStructs.h"

bool readFile(FILE* fp, char* fileName);   /* Read .am file for second pass
                                            * Input: fileName for error prints use and a pointer to the file
                                            * Output: error status */

bool secondPass(char* fileName, int *ICF, int *DCF); /* Manager function for second pass
                                                      * Input: file name, reffrence to final instruction counter
                                                      * and final data counter
                                                      * Output: true if error occurs */

bool isEntryInstruction(char *token);  /* Check if token is .entry
                                        * Input: pointer to string.
                                        * Output: true if the string is a .entry */

bool addEntryAttribute(const char* fileName, char* label, int lineNum); /* finds label according to the given name and adds
                                                    * an entry attribute to it
                                                    * Input: file name for error prints, label name, line number for error prints
                                                    * Output: true if a label wasnt found in list (was not declared as entry nor extern) or if already extern type */

bool UpdateWordsListNodes(const char* fileName);   /* Updates label list and words list according to the label list created
                                                    * Input: file name for error printd use
                                                    * Output: true if error found */


#endif
