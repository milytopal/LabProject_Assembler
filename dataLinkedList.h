//
// Created by Mily Topal on 06/02/2022.
//

#ifndef DATA_LINKED_LIST_H
#define DATA_LINKED_LIST_H
#include "dataStructs.h"

#define NODE_STR_LENGTH 20

typedef struct Node{
    char data[NODE_STR_LENGTH];
    struct Node *next;
} Node;

typedef struct List{
    Node *head;
    int length;
} List;


void add(List, Node);
Node* get(List, int);
void remove(List, int);


#endif //DATA_LINKED_LIST_H
