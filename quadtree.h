#ifndef _TREE_H_
#define _TREE_H_

#include "data.h"
#include "list.h"

typedef struct point2D point_t;
struct point2D {
    double x;
    double y;
};

typedef struct dataPoint dpoint_t;
struct dataPoint {
    node_t *node;
    point_t *point;
};

typedef struct rectangle2D rectangle_t;
struct rectangle2D {
    point_t *top_right;
    point_t *bot_left;
};

typedef struct quadtreeNode qtree_t;
struct quadtreeNode {
    rectangle_t *rec;
    dpoint_t *data_point;

    qtree_t *SW;
    qtree_t *NW;
    qtree_t *SE;
    qtree_t *NE;
};


int isEmpty(qtree_t *root);
int isSubdived(qtree_t *root);
int inRectangle(rectangle_t *rec, point_t *point);
int rectangleOverlap(rectangle_t *rec1, rectangle_t *rec2);
int insertNode(qtree_t *root, point_t *point, data_t *data);
int searchTree(qtree_t *root, point_t *point,FILE *file, char *x, char *y);
int rangeQuery(qtree_t *root, rectangle_t *rec, list_t *list, char* quad, FILE *file);

void freeTree(qtree_t *root);
void printPoint(point_t *point);
void subdivide(qtree_t *root, data_t *data);
void determineQuadrant(rectangle_t *rec, point_t *point);

qtree_t *newTree(rectangle_t *rec);
point_t *addPoint(double x, double y);
dpoint_t *createDataPoint(data_t *data, point_t *point);
rectangle_t *newRectangle(point_t *top_right, point_t *bot_left);

#endif
