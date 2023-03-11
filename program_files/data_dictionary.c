/*----------------------------------------------------------------------------
Project: dict3, dict4
data_dictionary.c : the implementation of the module dealing with dictionaries
of project data

A linked list was chosen as the dictionary implementation

Inspired by:
- Alistair Moffat's linked list implementation in the book Programming,
Problem Solving and Abstraction
- Linked list implementation in COMP20003 workshop exercise W4.6
----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "data.h"
#include "point_region_data.h"
#include "data_dictionary.h"

// reads all footpath records from .csv file "data_file" into footpath_t data,
// stores all data as nodes in a linked list dictionary of footpaths, and
// returns the pointer to this dictionary
linked_list_t *read_footpaths(FILE* data_file) {
    // read csv header line first
    while (getc(data_file) != '\n');

    linked_list_t *footpaths_dict = make_new_linked_list();

    footpath_t *footpath = read_single_footpath(data_file);
    while (footpath) {
        add_new_node(footpaths_dict, footpath);
        footpath = read_single_footpath(data_file);
    }

    return footpaths_dict;
}

// returns pointer to an empty linked list
linked_list_t *make_new_linked_list() {
    linked_list_t *linked_list;
    linked_list = (linked_list_t*) malloc(sizeof(*linked_list));
    assert(linked_list);

    linked_list->head = NULL;
    linked_list->tail = NULL;
    linked_list->num_nodes = 0;

    return linked_list;
}

// creates a new node containing "footpath" data and inserts it into the tail
// of "footpath_dict"
void add_new_node(linked_list_t *footpath_dict, footpath_t *footpath) {
    node_t *new_node = malloc(sizeof(*new_node));

    assert(footpath_dict);
    assert(new_node);

    new_node->startpoint.x = footpath->start_lon;
    new_node->startpoint.y = footpath->start_lat; 
    new_node->endpoint.x = footpath->end_lon;
    new_node->endpoint.y = footpath->end_lat;
    
    new_node->data = footpath;
    new_node->next = NULL;

    if (footpath_dict->tail == NULL) {
        footpath_dict->head = footpath_dict->tail = new_node;
    } else {
        footpath_dict->tail->next = new_node;
        footpath_dict->tail = new_node;
    }

    footpath_dict->num_nodes += 1;
}

// frees memory allocated to the linked list dictionary of footpaths
// "footpath_dict"
void free_footpath_dict(linked_list_t *footpath_dict) {
	assert(footpath_dict);

	node_t *curr = footpath_dict->head;
	while (curr) {            
		node_t *prev = curr;
		curr = curr->next; 
		free_footpath_data(prev->data);       
		free(prev);           
	}
    
	free(footpath_dict);
}
