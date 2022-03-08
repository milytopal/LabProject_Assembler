
#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "dataStructs.h"

/*------------ global variables ------------*/
/* head of macro list*/
pMacroNode head;

/*--------------- functions ----------------*/
/* the preprocess pass parsing the macros and placing them in the new file .am*/
bool PreProcessPass(const char* fileName, bool firstPass);
/* add a new macro to list */
pMacroNode addNewMacro(const char* name);
/* add new token the tokens list of a given macro */
void addLineToMacro(pMacroNode macro, char* line);
/* read source file and process data acording to the command lines */
bool foo(FILE *fp, FILE * newFp);
/* write all the tokens in the token list of the macro from head to end */
bool writeMacroToFile(pMacroNode macro, FILE *file);
/* delete the list in the end of process to free memory */
void deleteMacroList(pMacroNode head );
/* returns the macro node that matches the name */
pMacroNode findMacro( char* name);


#endif
