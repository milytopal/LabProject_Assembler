
#include "wordLinkedList.h"


void addWordNode( const Word word,const int address)
{
    pWordNode curr = NULL;
    if(wordsHead == NULL)
    {
        wordsHead = (pWordNode) calloc( sizeof(WordNode),1);
        wordsHead->word = word;
        wordsHead->word.address = address;
        wordsHead->pNext = NULL;
        printf(" %ld ",(long)(wordsHead) );
        return;
    }else
    {
        curr=wordsHead;
        /* go to end of list */
        while((curr->pNext) != NULL)
        {
            curr = curr->pNext;
        }
        curr->pNext = (pWordNode)calloc(sizeof(WordNode),1);
        curr->word = word;
        curr->word.address = address;
        curr->pNext->pNext = NULL;
        printf(" %d ",(int)(wordsHead->word.code.opcode) );
    }
}

void deleteWordList(const pWordNode head)
{
    pWordNode nextNode = NULL;
    while(wordsHead != NULL)
    {
        nextNode = wordsHead->pNext;
        free(wordsHead);
        wordsHead = nextNode;
    }
    wordsHead = NULL;
}

