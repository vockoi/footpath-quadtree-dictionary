/*----------------------------------------------------------------------------
Project: dict3, dict4
data_index.h : the implementation of the module dealing with indexes of
project data

- An array was chosen as the index implementation
- Function to sort the index was implemented with just the footpath_id
attribute in mind

Inspired by:
- Array implementation in COMP20003 workshop exercise W3.8
----------------------------------------------------------------------------*/

#ifndef _DATA_INDEX_H_
#define _DATA_INDEX_H_

// data definitions
typedef struct arr array_t;

struct arr {
	footpath_t **data_array; // array of pointers to footpath data
	int num_elements;		 // number of elements in "data_array"
	int size; 			     // size of array
};

/*---------------------------function definitions---------------------------*/

// returns pointer to an empty dynamic array of initial size 1
array_t *make_new_array();

// adds a new element "footpath_loc" into "index"'s data_array
void add_new_data(array_t *index, footpath_t *footpath_loc);

// adds all data in "index_2"'s data array into "index_1"
void add_new_data_array(array_t *index_1, array_t *index_2);

// prints footpaths stored in "index"'s data array in order of footpath id into
// "out_file"
void print_footpaths(array_t *index, FILE *out_file);

// sorts an unsorted index of footpath data "index" in ascending order using a
// comparison function "compareData" which compares values of a certain
// attribute between "footpath_1" and "footpath_2". 
// "compareData" always returns 0 if "footpath_1" has the lower attribute value
// than "footpath_2" and 1 otherwise
void sort_index(array_t *index, int (compareData(footpath_t *footpath_1,
footpath_t *footpath_2)));

// frees memory allocated to an index of footpath data "index"
void free_index(array_t *index);

#endif
