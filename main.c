#include <stdio.h>

#include "dataStructs.h"
#include "utils.h"
#include "preProcessor.h"
#include "firstPass.h"
#include "secondPass.h"
#include "parseCodeToFile.h"
#include "testParser.h"

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
            /* todo: startFirstPass(*argv, firstPass:false);*/
        }
        if(isErrorFound == false)
        {
            /* todo: parseCodeToFiles */
        }
        pWordNode test;
        test = createWordList();
        
        parseCodeToFile(argv[i],false,test);

        /* todo: reset linked listst and everything*/
    }

    return 0;
}
