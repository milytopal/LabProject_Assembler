#ifndef WORD_LINKED_LIST_H
#define WORD_LINKED_LIST_H
#include "dataStructs.h"
#include <stdio.h>
#include <stdlib.h>

void addWordNodeToCode(Word word, int val, char labelDest , int lineN); /* constructs a word node for the code segments and
 * adds it to the code list */

void addWordNodeToData(Word word, int val, char labelDest , int lineN); /* constructs a word node for the data segment and
 * adds it to the data list*/

void deleteWordList(pWordNode); /* frees up the lists created
 * Input: a given list's head  */
#endif
