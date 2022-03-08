#include <stdio.h>

#include "dataStructs.h"
#include "utils.h"
#include "preProcessor.h"
#include "firstPass.h"
#include "secondPass.h"
#include "parseCodeToFile.h"

bool isErrorFound; /* global varivable - represents if error found in current file*/

int main(int argc, char *argv[]) {
    int i;

    isErrorFound = false;
    if(argc < 2)
    {

    }
    for(i=2; i<argc ; i++)
    {
        PreProcessPass(argv[i],false);
        /* todo: startFirstPass(*argv, firstPass:true);*/
        if(isErrorFound == false)
        {
            firstPass(argv[i],false);
            /* todo: startFirstPass(*argv, firstPass:false);*/
        }
        if(isErrorFound == false)
        {
            parseCodeToFile(argv[i],false);
            /* todo: parseCodeToFiles */
        }
        /* todo: reset linked listst and everything*/
    }

    return 0;
}
