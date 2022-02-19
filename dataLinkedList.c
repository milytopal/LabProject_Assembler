
#include "dataLinkedList.h"

Node* get(List list, int index){
    if (index > list.length) {
        return;
    }

    Node *node = list.head;
    int i = 0;
    for (; i < index; ++i) {
        node = node->next;
    }
    return node;
}

void add(List list, Node toAdd){
    get(list, list.length)->next = &toAdd;
}

void remove(List list, int index) {
    Node *node = get(list, index-1);
    node->next = node->next->next;
}
