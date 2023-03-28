#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"

/* Sorts a linked list by the footpath_id
   Edited From: https://www.geeksforgeeks.org/c-program-bubble-sort-linked-list/
*/
void sortList(node_t *head) { 
    int swapped; 
    node_t *curr = NULL; 
    node_t *ptr = NULL; 
  
    /* Checking for empty list */
    if (head != NULL) {
        do { 
            swapped = 0; 
            curr = head; 
            while (curr->next != ptr) 
            { 
                if (curr->data->footpath_id > curr->next->data->footpath_id) 
                {  
                    data_t *temp = curr->data;
                    curr->data = curr->next->data;
                    curr->next->data = temp;
                    swapped = 1; 
                } 
                curr = curr->next; 
            } 
            ptr = curr; 
        } while(swapped);
    }
} 

/* Create a head node for a linkedlist
*/
node_t *createNode() {
    node_t *head = malloc(sizeof(node_t)); 
    head->next = NULL;
    return head;
}

/* Add node to the end of the linklist
   Edited from https://www.hackerearth.com/practice/data-structures/linked-list/singly-linked-list/tutorial/#:~:text=In%20C%20language%2C%20a%20linked,address%20of%20the%20next%20node.
*/
node_t *addNode(node_t *head, data_t *value) {
    node_t *newNode;
    node_t *nextNode; 

    newNode = createNode();
    newNode->data = value; 
    if(head == NULL){
        head = newNode;    
    }
    else{
        nextNode = head;
        while(nextNode->next != NULL){
            nextNode = nextNode->next;
        }
        nextNode->next = newNode;
    }
    return head;
}

// Free the memory of all the node and string 
void freeNode(node_t *head) {
    node_t *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp->data->address);
        free(tmp->data->clue_sa);
        free(tmp->data->asset_type);
        free(tmp->data->segside);
        free(tmp);
    }
}

/* Removes duplicate from linked list 
   Edited from: https: //www.geeksforgeeks.org/remove-duplicates-from-an-unsorted-linked-list/*/
void removeDuplicates(node_t *head) {
    node_t *node1;
    node_t *node2;
    node_t *dup;

    node1 = head;
    while (node1 != NULL && node1->next != NULL) {
        node2 = node1;
        while (node2->next != NULL) {
            /* check if they are duplicates */
            if (node1->data == node2->next->data) {
                dup = node2->next;
                node2->next = node2->next->next;
                free(dup);
            }
            else 
                node2 = node2->next;
        }
        node1 = node1->next;
    }
}

