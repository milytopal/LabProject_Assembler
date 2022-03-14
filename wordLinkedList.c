
#include "wordLinkedList.h"


void addWordNodeToCode(Word word, int val, char labelDest, int lineN)
{
    pWordNode curr = NULL;
    if(wordsHead == NULL)
    {
        wordsHead = (pWordNode) calloc( sizeof(WordNode),1);
        wordsHead->word = word;
        wordsHead->word.address = val;
        wordsHead->word.labelDest = labelDest;
        wordsHead->word.lineNum = lineN;
        wordsHead->pNext = NULL;
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
        curr->pNext->word = word;
        curr->pNext->word.address = val;
        curr->pNext->word.lineNum = lineN;
        curr->pNext->word.labelDest = labelDest;
        curr->pNext->pNext = NULL;
    }
}


void addWordNodeToData( const Word word,const int address, char labelDest,int lineN)
{
    pWordNode curr = NULL;
    if(datasHead == NULL)
    {
        datasHead = (pWordNode) calloc( sizeof(WordNode),1);
        datasHead->word = word;
        datasHead->word.address = address;
        datasHead->word.labelDest = labelDest;
        datasHead->word.lineNum = lineN;
        datasHead->pNext = NULL;
        return;
    }else
    {
        curr=datasHead;
        /* go to end of list */
        while((curr->pNext) != NULL)
        {
            curr = curr->pNext;
        }
        curr->pNext = (pWordNode)calloc(sizeof(WordNode),1);
        curr->pNext->word = word;
        curr->pNext->word.address = address;
        curr->pNext->word.lineNum = lineN;
        curr->pNext->word.labelDest = labelDest;
        curr->pNext->pNext = NULL;
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

