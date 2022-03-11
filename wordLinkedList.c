
#include "wordLinkedList.h"


void addWordNode( const Word word,const int address)
{
    pWordNode curr = NULL;
    if(wordsHead == NULL)
    {
        wordsHead = (pWordNode) calloc( sizeof(WordNode),1);
        //memcpy(&(wordsHead->word), &word, sizeof(Word));
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
        //memcpy(&(curr), &word, sizeof(Word));
        curr->word = word;
        curr->word.address = address;
        curr->pNext->pNext = NULL;
        printf(" %d ",(int)(wordsHead->word.code.opcode) );
    }
}
void deleteWordList(const pWordNode head)
{

}

