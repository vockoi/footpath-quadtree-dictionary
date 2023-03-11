/*----------------------------------------------------------------------------
Project: dict3, dict4
quadtree.h : the interface of the module dealing with quadtrees
----------------------------------------------------------------------------*/

#ifndef _QUADTREE_H_
#define _QUADTREE_H_

// flags to denote type of a quadtree node
#define EMPTY 0           // node is empty
#define INTERNAL_NODE 1   // node has 4 quadrant node children
#define LEAF 2            // node contains single datapoint

// data definitions
typedef struct qtree_node qtree_node_t;
typedef struct data_point data_point_t;

// data point storing "footpaths" which share a same "coordinate"
struct data_point {
    pointd_t coordinate; // pointer to coordinate of data point
    array_t *footpaths;  // array of addresses to footpaths with `coordinate`
};

// quadtree node
struct qtree_node {
    rectangle2d_t region;    // rectangular region covered by the node
    data_point_t *data;      // if the node is a leaf, stores a data point
    int flag;                // type of the quadtree node

    qtree_node_t *sw_region; // pointer to southwest region of node
    qtree_node_t *nw_region; // pointer to northwest region of node
    qtree_node_t *ne_region; // pointer to northeast region of node
    qtree_node_t *se_region; // pointer to southeast region of node
};

/*---------------------------function definitions---------------------------*/

// returns pointer to a new, initialized quadtree node
qtree_node_t *new_qtree_node();

// builds and returns pointer to the root of a PR quadtree of data from 
// "data_dictionary", with all points bounded by "region_boundary"
qtree_node_t *build_qtree(rectangle2d_t region_boundary, 
linked_list_t *data_dictionary);

// adds a data point with coordinates "point" and footpaths corresponding to
// the coordinates, "footpath" to the node "root" which has points bounded 
// within "region". how and where the data point is added to depends on 
// current state of the node.
void add_data_point(qtree_node_t *root, rectangle2d_t region, pointd_t point,
array_t *footpaths);

// helper function used within add_data_point. given the current node the data
// point is traversing in, "root", adds a data point with coordinates "point" 
// and footpaths corresponding to the coordinates, "footpath", to the quadrant
// of "root" represented by the int "quadrant"
void add_to_quadrant(int quadrant, qtree_node_t *root, pointd_t point,
array_t *footpaths);

// traverse a PR quadtree starting from "root" to find an internal node whose
// rectangular region would directly contain coordinates of "query_point".
// traverses into the leaf node of this internal node, stores footpaths
// associated with the node in "matches", then stops the search by returning
void search_qtree_point(qtree_node_t *root, pointd_t query_point, 
array_t **matches);

// traverse a PR quadtree starting from "root" to find all data points with
// coordinates that are within "region". if "root" is a leaf node, adds 
// footpath data associated with the node to "matches" and stop the search by 
// returning
void search_qtree_rectangle(qtree_node_t *root, rectangle2d_t region, 
array_t *matches);

// frees all space allocated to a quadtree node "root"
void free_qtree_node(qtree_node_t *root);

// frees all space allocated to a quadtree with root "root"
void free_qtree(qtree_node_t *root);

#endif
