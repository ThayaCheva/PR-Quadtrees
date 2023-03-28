#ifndef _DATA_H_
#define _DATA_H_

#include <stdlib.h>

#define MAX_CSV 1000
#define MAX_STRING 128
#define MAX_INPUT 512
#define MAX_FIELDS 19

struct data {
    int footpath_id;
    char *address;
    char *clue_sa;
    char *asset_type;
    double deltaz;
    double distance;
    double grade1in;
    int mcc_id;
    int mccid_int;
    double rlmax;
    double rlmin;
    char *segside;
    int statusid;
    int streetid;
    int street_group;
    double start_lat;
    double start_lon;
    double end_lat;
    double end_lon;
};
typedef struct data data_t;

void printData (data_t A[], FILE *file);
int readCSV(data_t *d, FILE *file);
int buildArray(data_t A[], FILE *file);
int read_input(double coords[]);

#endif
