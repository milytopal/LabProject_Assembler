
#ifndef FIRST_PASS_H
#define FIRST_PASS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "dataStructs.h"
#include "dataLinkedList.h"
#include "wordLinkedList.h"


bool firstPass(const char* fileName, int *ICF, int *DCF); /* first pass function */
bool labelCheck(const char*, char*, int); /* check if a given token meets the requirements as a label name,
 * and also strips the ':' from the label token
 * Input: file name and line number for the use of error prints and the label name (with the :)
 * Output: true if valid for a label check otherwise false */

bool isRegName(const char *label); /* checks if a given token is a register name using an array of the registers strings */
int getRegNum(const char *reg); /* gets the numeric value of a given register token, the strings array corresponds the
 * index af the registers therefore can be used as a map */

int getNumOfExpectedArguments(eCommands command); /* get the number of expected arguments according to the operation's
 * opcode for validation use */

char* getLabelFromIndexAddressing(char* token); /* separate the label from the brackets */

eCommands getOpcode(char*); /* get opcode according to operation, the opcode is set by the turned on bit in word */

int getFunct(int, char*); /* get the funct value of an operation */

bool fillOutArguments(const char*, char*,unsigned int ,int ,int ); /* handle the arguments after the operation,
 * Input: the whole string after the operation, the function dissects the string, fills out argument words and adds the
 * words to the list according to the addressing method and data, adds also data for use in second path such as label names
 * in case of direct or indexed addressing methods
 * Output: true if an error was found during the argument handling, the error prints are also handled inside*/

#endif
