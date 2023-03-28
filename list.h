#ifndef _LIST_H_
#define _LIST_H_

#include "data.h"

struct node {
    data_t *data;
    struct node* next;
}; 
typedef struct node node_t;

struct list {
    node_t *head;
};
typedef struct list list_t;

void freeNode(node_t *head);
void sortList(node_t *head);
void removeDuplicates(node_t *head);

node_t *createNode();
node_t *addNode(node_t *head, data_t *value);

#endif
