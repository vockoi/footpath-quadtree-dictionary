/*----------------------------------------------------------------------------
Project: dict3, dict4
data_index.c : the implementation of the module dealing with indexes of
project data

- An array was chosen as the index implementation
- Function to sort the index was implemented with just the footpath_id
attribute in mind

Inspired by:
- Array implementation in COMP20003 workshop exercise W3.8
----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "data.h"
#include "data_index.h"

// returns pointer to an empty dynamic array of starting size 1
array_t *make_new_array() {
    array_t *arr = malloc(sizeof(*arr));
    assert(arr);
    
	arr->data_array = malloc(sizeof(*(arr->data_array)));
	assert(arr->data_array);
	arr->num_elements = 0;
    arr->size = 1;
	
    return arr;
}

// adds a new element "footpath_loc" into "index"'s data_array
void add_new_data(array_t *index, footpath_t *footpath_loc) {
    // malloc more space for new data if not enough space in current array
    if (index->num_elements == index->size) {
        index->size *= 2;
        index->data_array = realloc(index->data_array, 
        index->size * sizeof(*(index->data_array)));
        assert(index->data_array);
    }

    // add new element to data array
    index->data_array[(index->num_elements)] = footpath_loc;
    index->num_elements++;
}

// adds all data in "index_2"'s data array into "index_1"
void add_new_data_array(array_t *index_1, array_t *index_2) {
    for (int i = 0; i < index_2->num_elements; i++) {
        add_new_data(index_1, (index_2->data_array)[i]);
    }
}

// prints footpaths stored in "index"'s data array in order of footpath id into
// "out_file"
void print_footpaths(array_t *index, FILE *out_file) {
    sort_index(index, has_smaller_id);

    int i;
    for (i = 0; i < index->num_elements; i++) {
        if (i == index->num_elements - 1) {
            print_footpath_data((index->data_array)[i], out_file);
        } 
        // only print one of duplicate footpaths, if there are any
        else if (!has_same_id((index->data_array)[i], 
        (index->data_array)[i + 1])) {
            print_footpath_data((index->data_array)[i], out_file);
        }
    }
}

// sorts an unsorted dictionary index "index" in ascending order using a
// comparison function "compareData" which compares values of a certain
// attribute between "footpath_1" and "footpath_2". 
// "compareData" always returns 0 if "footpath_1" has the lower attribute value
// than "footpath_2" and 1 otherwise
void sort_index(array_t *index, int (compareData(footpath_t *footpath_1,
footpath_t *footpath_2))) {
    footpath_t **footpath_locs = index->data_array;

    for (int i = 0; i < index->num_elements; i++) {
        for (int j = i + 1; j < index->num_elements; j++) {

            // if element in array has bigger attribute value than the element
            // after it, swap the two elements.
            if (compareData(footpath_locs[j], footpath_locs[i])) {
                footpath_t *temp =  footpath_locs[i];
                footpath_locs[i] = footpath_locs[j];
                footpath_locs[j] = temp;
            }
        }
    }
}

// frees memory allocated to the data index "index"
void free_index(array_t *index) {
    free(index->data_array);
    free(index);
}
