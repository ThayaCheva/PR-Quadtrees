#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"
#include "data.h"
#include "quadtree.h"


/* main.c
*
* Created by Thaya Chevaphatrakul (tchevaphatra.unimelb.edu.au) 8/17/2022
* 
* A program that implements a PR quadtree to store data points and to search for 
* them
* 
* To run the program for stage 3
* ./dict3 3 tests/dataset_1.csv output.out 144.969 -37.7975 144.971 -37.7955 < tests/test1.s3.in > output.stdout.out
* 
*  To run the program for stage4
* ./dict4 4 tests/dataset_1.csv output.out 144.969 -37.7975 144.971 -37.7955 < tests/test9.s4.in > output.stdout.out
* 
*
* Base code from Assignment 1
*/

int main(int argc, char *argv[]) {
    assert(argc >= 6);
    FILE *inFile = fopen(argv[2], "r");
    FILE *outFile = fopen(argv[3], "w");
    assert(inFile);
    assert(outFile);
    int stage;
    double bottomx, bottomy, topx, topy;
    data_t data[MAX_CSV+1];
    sscanf(argv[1], "%d", &stage);
    sscanf(argv[4], "%lf", &bottomx);
    sscanf(argv[5], "%lf", &bottomy);
    sscanf(argv[6], "%lf", &topx);
    sscanf(argv[7], "%lf", &topy);

    // Count the number of elements in the csv file
    int n = buildArray(data, inFile);

    // Add data to linked list
    list_t *csv_list = malloc(sizeof(list_t));
    assert(csv_list);
    csv_list->head = NULL;
    for (int i = 0; i < n; i++) {
        csv_list->head = addNode(csv_list->head, &data[i]);
    }

    // Create new Quadtree
    qtree_t *root = newTree(newRectangle(addPoint(topx, topy), addPoint(bottomx, bottomy)));


    // Loops through all the data and insert them into the quadtree
    node_t *curr = csv_list->head;
    while (curr != NULL) {
        point_t *start_coord = addPoint(curr->data->start_lon, curr->data->start_lat);
        point_t *end_coord = addPoint(curr->data->end_lon, curr->data->end_lat);
        insertNode(root, start_coord, curr->data);

        insertNode(root, end_coord, curr->data);

        curr = curr->next;
    }
    
    // Stage 3
    if (stage == 3) {
        char x1[MAX_STRING];
        char y1[MAX_STRING];
        double x;
        double y;
        // Search through the tree for the location of the point
        while (scanf("%s %s", x1, y1) == 2) {
            sscanf(x1, "%lf", &x);
            sscanf(y1, "%lf", &y);
            printf("%s %s -->", x1, y1);

            point_t *newPoint = addPoint(x, y);
            searchTree(root, newPoint, outFile, x1, y1);

            printf("\n");
            free(newPoint);
        }
    }

    if (stage == 4) {
        char x1_t[MAX_STRING];
        char y1_t[MAX_STRING];
        char x2_t[MAX_STRING];
        char y2_t[MAX_STRING];
        double x1;
        double y1;
        double x2;
        double y2;
        while (scanf("%s %s %s %s", x1_t, y1_t, x2_t, y2_t) == 4) {
            list_t *list = malloc(sizeof(list_t));
            assert(list);
            list->head = NULL;
        
            sscanf(x1_t, "%lf", &x1);
            sscanf(y1_t, "%lf", &y1);
            sscanf(x2_t, "%lf", &x2);
            sscanf(y2_t, "%lf", &y2);

            // Perform range query and insert data to list
            printf("%s %s %s %s -->", x1_t, y1_t, x2_t, y2_t);
            fprintf(outFile, "%s %s %s %s\n", x1_t, y1_t, x2_t, y2_t);
            rectangle_t *boundary = newRectangle(addPoint(x2, y2), addPoint(x1, y1));
            rangeQuery(root, boundary, list, NULL, outFile);

            // Print out data to outfile sorted and without duplicate
            sortList(list->head);
            removeDuplicates(list->head);
            node_t *curr = list->head;

            while (curr != NULL) {
                printData(curr->data, outFile);
                curr = curr->next;
            }

            printf("\n");

            node_t *tmp = list->head;
            node_t *prev;
            while (tmp != NULL) {
                prev = tmp;
                tmp = tmp->next;
                free(prev);
            }

            free(boundary->top_right);
            free(boundary->bot_left);
            free(boundary);
            free(list);
        }
    }

    freeTree(root);
    freeNode(csv_list->head);
    free(csv_list);
    fclose(inFile);
    fclose(outFile);
    return 0;
}

