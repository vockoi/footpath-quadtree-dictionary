/*
Project: dict3, dict4
data_dictionary.h : the interface of the module dealing with dictionaries
of project data

A linked list was chosen as the dictionary implementation

Inspired by:
- Alistair Moffat's linked list implementation in the book Programming,
Problem Solving and Abstraction
- Linked list implementation in COMP20003 workshop exercise W4.6
----------------------------------------------------------------------------*/

#ifndef _DATA_DICTIONARY_H_
#define _DATA_DICTIONARY_H_

//data definitions
typedef struct node node_t;
typedef struct linked_list linked_list_t;

// a linked list node
struct node {
    pointd_t startpoint; // startpoint of footpath's line approximation
    pointd_t endpoint;   // endpoint of footpath's line approximation
    footpath_t *data;    // pointer to footpath data stored in the node
    node_t *next;        // pointer to next node in linked list dictionary
};

struct linked_list {
    node_t *head;   // first node in linked list dictionary
    node_t *tail;   // last node in linked list dictionary
    int num_nodes;  // number of nodes in linked list dictionary
};

/*---------------------------function definitions---------------------------*/

// reads all footpath records from .csv file "data_file" into footpath_t data,
// stores all data as nodes in a linked list dictionary of footpaths, and
// returns the pointer to this dictionary
linked_list_t *read_footpaths(FILE* data_file);

// returns pointer to an empty linked list
linked_list_t *make_new_linked_list();

// creates a new node containing "footpath" data and inserts it into the tail
// of "footpath_dict"
void add_new_node(linked_list_t *footpath_dict, footpath_t *footpath);

// frees memory allocated to the linked list dictionary of footpaths
// "footpath_dict"
void free_footpath_dict(linked_list_t *footpath_dict);

#endif
