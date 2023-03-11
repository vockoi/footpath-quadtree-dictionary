/*----------------------------------------------------------------------------
Project: dict3, dict4
data.h : the interface of the module dealing with project data ('footpaths')
----------------------------------------------------------------------------*/

#ifndef _DATA_H_
#define _DATA_H_

// constant definitions
#define MAX_STRING_LEN 128 // max char length of string-type fields
#define SCAN_SUCCESS 1     // successfully read a footpath record

// data definitions
typedef struct footpath footpath_t;

// footpath data
struct footpath {
	int footpath_id;  // row number of the footpath
	char *address;    // footpath location
	char *clue_sa;    // CLUE small area the footpath is in
	char *asset_type; // type of footpath
	double deltaz;    // change in vertical distance along footpath
	double distance;  // length of footpath in metres
	double grade1in;  // percentage gradient of footpath
	int mcc_id; 	  // footpath id number
	int mccid_int; 	  // id number of road or intersection the footpath borders
	double rlmax;     // highest elevation on footpath
	double rlmin;     // lowest elevation on footpath
	char *segside;    // side of the road footpath is on
	int statusid;     // status of the footpath
	int streetid;     // ID of first street in the footpath address
	int street_group; // id of a footpath connected to this footpath with no gap
	double start_lat; // latitude of startpoint of footpath's line approximation
	double start_lon; // longitude of startpoint of footpath's line approximation
	double end_lat;   // latitude of endpoint of footpath's line approximation
	double end_lon;   // longitude of endpoint of footpath's line approximation
};

/*---------------------------function definitions---------------------------*/

// reads a single footpath record from .csv file "data_file" to build one
// footpath_t data. returns pointer to data or NULL if reading is unsuccessful
footpath_t *read_single_footpath(FILE *data_file);

// reads a string-type field from a footpath record being scanned from
// "data_file" into "field" in the process of building footpath_t data
void read_string(FILE *data_file, char *field);

// prints a footpath record "footpath" to "outfile"
void print_footpath_data(footpath_t *footpath, FILE *out_file);

// returns 1 if "footpath_1" has smaller id number than "footpath_2" and 0
// otherwise
int has_smaller_id(footpath_t *footpath_1, footpath_t *footpath_2);

// returns 1 if "footpath_1" has same id number than "footpath_2" and 0
// otherwise
int has_same_id(footpath_t *footpath_1, footpath_t *footpath_2);

// frees memory allocated to a footpath "footpath_data"
void free_footpath_data(footpath_t *footpath_data);

#endif
