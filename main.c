#include <stdio.h>


#include "extrnVariables.h"
#include "utils.h"
bool isErrorFound; /* global varivable - represents if error found in current file*/

int main(int argc, char *argv[]) {
    isErrorFound = false;
    int i;
    if(argc < 2)
    {

    }
    for(i=2; i<argc ; i++)
    {
        /* todo: startFirstPass(*argv, firstPass:true);*/
        if(isErrorFound == false)
        {
            /* todo: startFirstPass(*argv, firstPass:false);*/
        }
        if(isErrorFound == false)
        {
            /* todo: parseCodeToFiles */
        }
        /* todo: reset linked listst and everything*/
    }

    return 0;
}
