#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "quadtree.h"
#include "data.h"

// Create a new point with x and y
point_t *addPoint(double x, double y) {
    point_t *point = malloc(sizeof(point_t));
    assert(point);
    point->x = x;
    point->y = y;
    return point;
}

// Print out the point
void printPoint(point_t *point) {
    printf("%lf %lf\n", point->x, point->y);
}

// Create a new rectangle
rectangle_t *newRectangle(point_t *top_right, point_t *bot_left) {
    rectangle_t *rec = malloc(sizeof(rectangle_t));
    assert(rec);
    rec->top_right = top_right;
    rec->bot_left = bot_left;
    return rec;
}

// Check if a point is in a rectangle
int inRectangle(rectangle_t *rec, point_t *point) {
    if (point->x > rec->bot_left->x && point->x < rec->top_right->x && point->y > rec->bot_left->y && point->y < rec->top_right->y) {
        return 1;
    }
    return 0;
}

/* Check if two rectangles overlap each other
   Edited From: https://www.geeksforgeeks.org/find-two-rectangles-overlap/ 
*/
int rectangleOverlap(rectangle_t *rec1, rectangle_t *rec2) {
    // if rectangles overlap on the side
    if (rec1->top_right->x < rec2->bot_left->x ||
        rec2->top_right->x < rec1->bot_left->x) {
        return 0;
    }
    // if rectangles overlap on top or bottom
    if (rec1->top_right->y < rec2->bot_left->y ||
        rec2->top_right->y < rec1->bot_left->y) {
        return 0;
    }
    
    return 1;
}

// Create a new quadtree
qtree_t *newTree(rectangle_t *rec) {
    qtree_t *qtree = malloc(sizeof(qtree_t));
    assert(qtree);
    qtree->rec = rec;

    qtree->SW = NULL;
    qtree->NW = NULL;
    qtree->SE = NULL;
    qtree->NE = NULL;

    qtree->data_point = NULL;

    return qtree;
}

// Create data point with a point and data
dpoint_t *createDataPoint(data_t *data, point_t *point) {
    dpoint_t *d = malloc(sizeof(dpoint_t));
    assert(d);
    d->point = point;
    d->node = NULL;
    return d;
}

/* Insert the point and data into the quadtree
   Edited From: https://www.youtube.com/watch?v=OJxEcs0w_kE&t=1501s
*/ 
int insertNode(qtree_t *root, point_t *point, data_t *data) {
    if (!(inRectangle(root->rec, point))) {
        return 0;
    }

    if (isEmpty(root) && !isSubdived(root)) {
        root->data_point = createDataPoint(data, point);
        root->data_point->node = addNode(root->data_point->node, data);
        return 1;
    } 

    // Check if data are in the same point, if so add to end of list  
    if (!isEmpty(root) && root->data_point->point->x == point->x && root->data_point->point->y == point->y && root->NW == NULL) {
        root->data_point->node = addNode(root->data_point->node, data);
        free(point);
        point = NULL;
        return 1;
    }

    
    if (!isSubdived(root)) {
        subdivide(root, data);
    }
    
    if (insertNode(root->NW, point, data)) {
        return 1;
    }

    if (insertNode(root->NE, point, data)) {
        return 1;
    }

    if (insertNode(root->SW, point, data)) {
        return 1;
    }

    if (insertNode(root->SE, point, data)) {
        return 1;
    }
    return 0;
}

/* Subdivide a rectangle into 4 quadrants
   Edited From: https://www.youtube.com/watch?v=OJxEcs0w_kE&t=1501s
*/
void subdivide(qtree_t *root, data_t *data) {
    double mid_x = (root->rec->top_right->x + (root->rec->bot_left->x))/2;
    double mid_y = (root->rec->top_right->y + (root->rec->bot_left->y))/2;

    // Subdivide into NW section
    point_t *top_nw = addPoint(mid_x, root->rec->top_right->y);
    point_t *bot_nw = addPoint(root->rec->bot_left->x, mid_y);
    root->NW = newTree(newRectangle(top_nw, bot_nw));

    // Subdivide into NE section
    point_t *top_ne = addPoint(root->rec->top_right->x, root->rec->top_right->y);
    point_t *bot_ne = addPoint(mid_x, mid_y);
    root->NE = newTree(newRectangle(top_ne, bot_ne));

    // Subdivide into SW section
    point_t *top_sw = addPoint(mid_x, mid_y);
    point_t *bot_sw = addPoint(root->rec->bot_left->x, root->rec->bot_left->y);
    root->SW = newTree(newRectangle(top_sw, bot_sw));

    // Subdivide into SE section
    point_t *top_se = addPoint(root->rec->top_right->x, mid_y);
    point_t *bot_se = addPoint(mid_x, root->rec->bot_left->y);
    root->SE = newTree(newRectangle(top_se, bot_se));

    // Move the node to another level and set to gray node
    node_t *curr = root->data_point->node;
    while (curr != NULL) {
        // Insert NW
        if (insertNode(root->NW, root->data_point->point ,curr->data)) {
            curr = curr->next;
        }
        // Insert NE
        else if (insertNode(root->NE, root->data_point->point ,curr->data)) {
            curr = curr->next;
        }
        // Insert SW
        else if (insertNode(root->SW, root->data_point->point ,curr->data)) {
            curr = curr->next;
        }
        // Insert SE
        else if (insertNode(root->SE, root->data_point->point ,curr->data)) {
            curr = curr->next;
        }     
    }

    // Free the data in the grey node
    node_t *tmp = root->data_point->node;
    node_t *prev;
    while (tmp != NULL) {
        prev = tmp;
        tmp = tmp->next;
        free(prev);
    }
    free(root->data_point);

    root->data_point = NULL;
} 

/* Determine which quadrant of a rectangle a point lies in
   Edited From: https://www.w3resource.com/c-programming-exercises/conditional-statement/c-conditional-statement-exercises-9.php
*/
void determineQuadrant(rectangle_t *rec, point_t *point) {
    double mid_x = (rec->top_right->x + (rec->bot_left->x))/2;
    double mid_y = (rec->top_right->y + (rec->bot_left->y))/2;

    if (point->x < mid_x && point->y < mid_y) {
        // SW
        printf("%s", " SW");
        
    }
    else if (point->x < mid_x && point->y > mid_y) {
        // NW
        printf("%s", " NW");
        
    }
    else if (point->x > mid_x && point->y > mid_y) {
        // NE
        printf("%s", " NE");
        
    }
    else if (point->x > mid_x && point->y < mid_y) {
        // SE
        printf("%s", " SE");
        
    }  
}

/* Search the location of a point within the quadtree 
   and display the path taken */
int searchTree(qtree_t *root, point_t *point, FILE *file, char *x, char *y) {
    if (!(inRectangle(root->rec, point))) {
        return 0;
    }

    if (!isSubdived(root) && !isEmpty(root)) {
        fprintf(file, "%s %s\n", x, y);
        
        sortList(root->data_point->node);
        
        node_t *curr = root->data_point->node;
        while (curr != NULL) {
            printData(curr->data, file);
            curr = curr->next;
        }
        
        return 0;
    }

    // Print the quadrant
    determineQuadrant(root->rec, point);

    searchTree(root->NW, point, file, x, y);

    searchTree(root->NE, point, file, x, y);

    searchTree(root->SW, point, file, x, y);

    searchTree(root->SE, point, file, x, y);

    return 0;
}

/* takes a 2D rectangle as argument and returns all datapoints in 
   the PR quadtree whose coordinates lie within the query rectangle
   Edited From: https://en.wikipedia.org/wiki/Quadtree
*/
int rangeQuery(qtree_t *root, rectangle_t *rec, list_t *list, char* quad, FILE *file) {

    if(!(rectangleOverlap(root->rec, rec))) {
        return 0;
    }

    if (!isEmpty(root) || isSubdived(root)) {
        if (quad != NULL) {
            printf(" %s", quad);                                                                                                                                         
        }
        // Only record if data exist
        if(!isEmpty(root)){

            node_t *curr;
            curr = root->data_point->node;
            while (curr != NULL) {
                // Check if point exist within rectangle
                if (inRectangle(rec, root->data_point->point)) {
                    list->head = addNode(list->head, curr->data); 
                }
                curr = curr->next;
            }
        }
    }

    if (!isSubdived(root)) {
        return 1;
    }

    rangeQuery(root->SW, rec, list, "SW", file);
        
    rangeQuery(root->NW, rec, list, "NW", file);
        
    rangeQuery(root->NE, rec, list, "NE", file);
         
    rangeQuery(root->SE, rec, list, "SE", file);
        
    return 1;
}

// Checks if a node is a grey node
int isSubdived(qtree_t *root) {
    if (root->NW == NULL) {
        return 0;
    }
    return 1;
}

// Checks if a node is empty
int isEmpty(qtree_t *root) {
    if (root->data_point == NULL) {
        return 1;
    }
    return 0;
}

// Free all the allocated memory in the tree
void freeTree(qtree_t *root) {
    if (root != NULL) {
        if(root->data_point != NULL) {
            node_t *tmp = root->data_point->node;
            node_t *prev;
            while (tmp != NULL) {
                prev = tmp;
                tmp = tmp->next;
                free(prev);
            }
            
            free(root->data_point->point);        
            free(root->data_point);
        }

        // Free all the leaf nodes
        freeTree(root->NW);
        freeTree(root->SW);
        freeTree(root->NE);
        freeTree(root->SE);

        // Free rectangles
        free(root->rec->top_right);
        free(root->rec->bot_left);
        free(root->rec);

        free(root);
    }
}


