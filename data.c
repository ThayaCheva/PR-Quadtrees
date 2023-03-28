#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "data.h"

// Print the data in the proper format
void printData(data_t A[], FILE *file) {
    fprintf(file, "--> footpath_id: %d || address: %s || clue_sa: %s || asset_type: %s || deltaz: %0.2lf || distance: %0.2lf || grade1in: %0.1lf || mcc_id: %d || mccid_int: %d || rlmax: %0.2lf || rlmin: %0.2lf || segside: %s || statusid: %d || streetid: %d || street_group: %d || start_lat: %lf || start_lon: %lf || end_lat: %lf || end_lon: %lf || \n", A->footpath_id, A->address, A->clue_sa, A->asset_type, A->deltaz, A->distance, A->grade1in, A->mcc_id, A->mccid_int, A->rlmax, A->rlmin, A->segside, A->statusid, A->streetid, A->street_group, A->start_lat, A->start_lon, A->end_lat, A->end_lon);
}

// Read the CSV files and parse them into the correct fields
int readCSV(data_t *d, FILE *file) {
    char line[MAX_INPUT + 1];
    char string[MAX_FIELDS][MAX_STRING] = {0};
    int n = 0, i = 0, j = 0, isQuote = 0;
    if(fgets (line, MAX_INPUT, file) != NULL) {
        while(line[n] != '\0') {
            // Searches for the beginning ,"
            if (line[n] == ',' && line[n+1] == '"') {
                isQuote = 1;
                n++;
            }
            // Searches for the ending ",
            else if (line[n] == '"' && line[n+1] == ','){
                isQuote = 0;
                n++;
            }
            // Determines if the text contains ""
            if (isQuote == 0) {
                if (line[n] != ',') {
                    string[j][i] = line[n];
                    i++;
                }
                else {
                    i = 0;
                    j++;
                }
            }
            else {
                if (line[n] != '"') {
                    string[j][i] = line[n];
                    i++;
                }
                else {
                    i = 0;
                    j++;
                }
            }
            n++;
        }
    }
    // Makes sure no fields are empty and parse all the values into their respective fields
    if (j == 18) {
        sscanf(string[0], "%d", &d->footpath_id);
        d->address = strdup(string[1]);
        assert(d->address);
        d->clue_sa = strdup(string[2]);
        assert(d->clue_sa);
        d->asset_type = strdup(string[3]);
        assert(d->asset_type);
        sscanf(string[4], "%lf", &d->deltaz);
        sscanf(string[5], "%lf", &d->distance);
        sscanf(string[6], "%lf", &d->grade1in);
        sscanf(string[7], "%d", &d->mcc_id);
        sscanf(string[8], "%d", &d->mccid_int);
        sscanf(string[9], "%lf", &d->rlmax);
        sscanf(string[10], "%lf", &d->rlmin);
        d->segside = strdup(string[11]);
        assert(d->segside);
        sscanf(string[12], "%d", &d->statusid);
        sscanf(string[13], "%d", &d->streetid);
        sscanf(string[14], "%d", &d->street_group);
        sscanf(string[15], "%lf", &d->start_lat);
        sscanf(string[16], "%lf", &d->start_lon);
        sscanf(string[17], "%lf", &d->end_lat);
        sscanf(string[18], "%lf", &d->end_lon);
        return 1;
    }
    return 0;
}

/* Reads data from file and build an array 
   and return the number of elements
   from W2.5 */
int buildArray(data_t A[], FILE *file) {
	int n;
	char line[MAX_INPUT + 1];
	
	// gets rid of the header line
	fscanf(file, "%[^\n] ", line);
	// we can also use: while (getchar()!='\n');

	// reads and stores students in array students
	for (n = 0; n < MAX_CSV && readCSV(&A[n], file); n++);
	return n;
}

/* Reads data from stdin and count the amount of input
   from W0.5 */
int read_input(double coords[]) {
	int n = 0;  
	for ( ; n < MAX_CSV && scanf(" %lf", &coords[n]) == 1; n++);
	return n;
}






