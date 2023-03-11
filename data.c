/*----------------------------------------------------------------------------
Project: dict3, dict4
data.c : the implementation of the module dealing with project data 
('footpaths')
----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "data.h"

// reads a single footpath record from .csv file "data_file" to build one
// footpath_t data. returns pointer to data or NULL if reading is unsuccessful
footpath_t *read_single_footpath(FILE *data_file) {
	int footpath_id;
	double deltaz, distance, grade1in, rlmax, rlmin, start_lat, start_lon,
	end_lat, end_lon;
	char address[MAX_STRING_LEN + 1], clue_sa[MAX_STRING_LEN + 1],
	asset_type[MAX_STRING_LEN + 1], segside[MAX_STRING_LEN + 1];
	// these fields are int-type but are represented as doubles in the .csv data
	// files. declare as double for scanning
	double mcc_id, mccid_int, statusid, streetid, street_group;
	
	// continue building footpath data if first component for the structure is
	// successfully scanned
	if (fscanf(data_file, "%d,", &footpath_id) == SCAN_SUCCESS) {
		read_string(data_file, address);
		read_string(data_file, clue_sa);
		read_string(data_file, asset_type);
		
		fscanf(data_file, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,",
		&deltaz, &distance, &grade1in, &mcc_id, &mccid_int, &rlmax, &rlmin);
		
		read_string(data_file, segside);
		
		fscanf(data_file, "%lf,%lf,%lf,%lf,%lf,%lf,%lf", &statusid, &streetid,
		&street_group, &start_lat, &start_lon, &end_lat, &end_lon);
		
		footpath_t *footpath = malloc(sizeof(*footpath)); 
		assert(footpath);

		footpath->footpath_id = footpath_id;
		footpath->address = strdup(address);
		assert(footpath->address);
		footpath->clue_sa = strdup(clue_sa);
		assert(footpath->clue_sa);
		footpath->asset_type = strdup(asset_type);
		assert(footpath->asset_type);
		footpath->deltaz = deltaz; 
		footpath->distance = distance;
		footpath->grade1in = grade1in;
		footpath->mcc_id = (int) mcc_id;
		footpath->mccid_int = (int) mccid_int;
		footpath->rlmax = rlmax;
		footpath->rlmin = rlmin;
		footpath->segside = strdup(segside);
		assert(footpath->segside);
		footpath->statusid = (int) statusid;
		footpath->streetid = (int) streetid;
		footpath->street_group = (int) street_group;
		footpath->start_lat = start_lat;
		footpath->start_lon = start_lon;
		footpath->end_lat = end_lat;
		footpath->end_lon = end_lon;

		return footpath;
	}
	
	return NULL;
}

// reads a string-type field from a footpath record being scanned from
// "data_file" into "field" in the process of building footpath_t data
void read_string(FILE *data_file, char *field) {
	char c = getc(data_file);
	
	// don't store quotation marks in "field" if the string data contains any
	int in_quotations = 0;
	if (c == '"') {
		in_quotations = 1;
		c = getc(data_file);
	}

	// only stop scanning if a comma OUTSIDE quotation marks is encountered
	while (c != ',' || in_quotations) {
		if (in_quotations && c == '"') {
			in_quotations = 0;

			// again, don't store the quotation mark
			c = getc(data_file);
			continue;
		}

		// copy c to position "field" and move to next position to build string
		*field = c;
		field++;
		c = getc(data_file);
	}

	// mark the end for string "field"
	*field = '\0';
}

// prints a footpath record "footpath" to "outfile"
void print_footpath_data(footpath_t *footpath, FILE *out_file) {
	fprintf(out_file, "--> footpath_id: %d || address: %s || clue_sa: %s"
	" || asset_type: %s || deltaz: %.2lf || distance: %.2lf || grade1in: %.1lf"
	" || mcc_id: %d || mccid_int: %d || rlmax: %.2lf || rlmin: %.2lf"
	" || segside: %s || statusid: %d || streetid: %d || street_group: %d"
	" || start_lat: %.6lf || start_lon: %.6lf || end_lat: %.6lf"
	" || end_lon: %.6lf || \n",
	footpath->footpath_id, footpath->address, footpath->clue_sa,
	footpath->asset_type, footpath->deltaz, footpath->distance,
	footpath->grade1in, footpath->mcc_id, footpath->mccid_int, footpath->rlmax,
	footpath->rlmin, footpath->segside, footpath->statusid, footpath->streetid,
	footpath->street_group, footpath->start_lat, footpath->start_lon,
	footpath->end_lat, footpath->end_lon);
}

// returns 1 if "footpath_1" has smaller id number than "footpath_2" and 0
// otherwise
int has_smaller_id(footpath_t *footpath_1, footpath_t *footpath_2) {
	return footpath_1->footpath_id < footpath_2->footpath_id;
}

// returns 1 if "footpath_1" has same id number than "footpath_2" and 0
// otherwise
int has_same_id(footpath_t *footpath_1, footpath_t *footpath_2) {
	return (footpath_1->footpath_id - footpath_2->footpath_id == 0);
}

// free memory allocated to a footpath "footpath_data"
void free_footpath_data(footpath_t *footpath_data) {
    free(footpath_data->address);
    free(footpath_data->clue_sa);
    free(footpath_data->asset_type);
    free(footpath_data->segside);
	free(footpath_data);
}
