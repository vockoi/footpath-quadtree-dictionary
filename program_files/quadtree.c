/*----------------------------------------------------------------------------
Project: dict3, dict4
quadtree.c : the implementation of the module dealing with quadtrees
----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "data.h"
#include "point_region_data.h"
#include "data_dictionary.h"
#include "data_index.h"
#include "quadtree.h"

enum quadrant {SW = 0, NW = 1, NE = 2, SE = 3};

// returns pointer to a new, initialized quadtree node
qtree_node_t *new_qtree_node() {
    qtree_node_t *new_node = malloc(sizeof(qtree_node_t));
    assert(new_node);

    new_node->flag = EMPTY;
    new_node->data = NULL;
    new_node->sw_region = NULL;
    new_node->nw_region = NULL;
    new_node->ne_region = NULL;
    new_node->se_region = NULL;

    return new_node;
}

// builds and returns pointer to the root of a PR quadtree of data from
// "data_dictionary", with all points bounded by "region_boundary"
qtree_node_t *build_qtree(rectangle2d_t region_boundary, 
linked_list_t *data_dictionary) {
    // root of new quadtree
    qtree_node_t *root = new_qtree_node();
    
    // iterate through each footpath in the data dictionary
    node_t *curr = data_dictionary->head;
    while (curr != NULL) {
        // add pointer to footpath into the quadtree at "root" as a single
        // element array; use footpath's starting latitude and longitude as 
        // key
        array_t *footpath_startpoint = make_new_array();
        add_new_data(footpath_startpoint, curr->data);
        add_data_point(root, region_boundary, curr->startpoint, 
        footpath_startpoint);

        // add pointer to footpath into the quadtree at "root" as a single
        // element array, but this time use footpath's ending latitude and 
        // longitude as key
        array_t *footpath_endpoint = make_new_array();
        add_new_data(footpath_endpoint, curr->data); 
        add_data_point(root, region_boundary, curr->endpoint,
        footpath_endpoint);

        curr = curr->next;
    }

    return root;
}

// adds a data point with coordinates "point" and footpaths corresponding to
// the coordinates, "footpath" to the node "root" which has points bounded 
// within "region". how and where the data point is added to depends on 
// current state of the node. return when data point is successfully added
// to an empty node.
void add_data_point(qtree_node_t *root, rectangle2d_t region, pointd_t point,
array_t *footpaths) {
    data_point_t *data_point = root->data;

    // found an empty node. insert the data point and update the node as a new 
    // leaf node
    if (root->flag == EMPTY) {
        root->region = region;
        data_point = malloc(sizeof(data_point_t));
        assert(data_point);

        data_point->coordinate = point;
        data_point->footpaths = footpaths;
        root->data = data_point;
        root->flag = LEAF;
        return;
    }

    // found a leaf node containing a data point with matching coordinates 
    // to the footpath being added. add pointer to footpath data into data
    // point's array of footpaths
    else if (root->flag == LEAF && is_same_point(data_point->coordinate,
    point)) {
        add_new_data(data_point->footpaths, (footpaths->data_array[0]));
        assert(data_point->footpaths);
        free_index(footpaths);
        return;
    }

    // current node "root" is an internal node or contains a data point with
    // non-matching coordinates to footpath being added
    else {
        // determine quadrant of the current node the new footpath would fall 
        // into and add data to the quadrant node
        int new_footpath_quadrant = determine_quadrant(root->region, point);
        add_to_quadrant(new_footpath_quadrant, root, point, footpaths);

        // in the case of "root" being a leaf node, need additional step of
        // moving old data point stored within the leaf node into quadrant node
        if (root->flag == LEAF) {
            int old_footpaths_quadrant = determine_quadrant(root->region,
            data_point->coordinate);
            add_to_quadrant(old_footpaths_quadrant, root, 
            data_point->coordinate, data_point->footpaths);

            // node is now an internal node. indicate with flag and free
            // data point data
            free(root->data);
            root->data = NULL;
            root->flag = INTERNAL_NODE;
        }
    }
}

// helper function used within add_data_point. given the current node the data
// point is traversing in, "root", adds a data point with coordinates "point" 
// and footpaths corresponding to the coordinates, "footpath", to the quadrant
// of "root" represented by the int "quadrant"
void add_to_quadrant(int quadrant, qtree_node_t *root, pointd_t point,
array_t *footpaths) {
    switch (quadrant) {
        case SW:
            // initialize quadrant node if it's null
            if (root->sw_region == NULL) {
                root->sw_region = new_qtree_node();
            }
            add_data_point(root->sw_region, get_quadrant(root->region, SW), 
            point, footpaths);
            break;
        case NW:
            if (root->nw_region == NULL) {
                root->nw_region = new_qtree_node();
            }
            add_data_point(root->nw_region, get_quadrant(root->region, NW), 
            point, footpaths);
            break;
        case NE:
            if (root->ne_region == NULL) {
                root->ne_region = new_qtree_node();
            }
            add_data_point(root->ne_region, get_quadrant(root->region, NE), 
            point, footpaths);
            break;
        case SE:
            if (root->se_region == NULL) {
                root->se_region = new_qtree_node();
            }
            add_data_point(root->se_region, get_quadrant(root->region, SE), 
            point, footpaths);
            break;
    }
}

// traverse a PR quadtree starting from "root" to find an internal node whose
// rectangular region would directly contain coordinates of "query_point".
// traverses into the leaf node of this internal node, stores footpaths
// associated with the node in "matches", then stops the search by returning
void search_qtree_point(qtree_node_t *root, pointd_t query_point, 
array_t **matches) {
    int quadrant = determine_quadrant(root->region, query_point);

    switch (quadrant) {
        case SW:
            // if quadtree can't be traversed any further, we found a match
            if (root->sw_region == NULL) {
                *matches = root->data->footpaths;
                return;
            } else {
                // continue searching
                printf(" SW");
                search_qtree_point(root->sw_region, query_point, matches);
                break;
            }
        case NW:
            if (root->nw_region == NULL) {
                *matches = root->data->footpaths;
                return;
            } else {
                printf(" NW");
                search_qtree_point(root->nw_region, query_point, matches);
                break;
            }
        case NE:
            if (root->ne_region == NULL) {
                *matches = root->data->footpaths;
                return;
            } else {
                printf(" NE");
                search_qtree_point(root->ne_region, query_point, matches);
                break;
            }
        case SE:
            if (root->se_region == NULL) {
                *matches = root->data->footpaths;
                return;
            } else {
                printf(" SE");
                search_qtree_point(root->se_region, query_point, matches);  
                break;
            }
    }

    return;
}

// traverse a PR quadtree starting from "root" to find all data points with
// coordinates that are within "region". if "root" is a leaf node, adds 
// footpath data associated with the node to "matches" and stop the search by 
// returning
void search_qtree_rectangle(qtree_node_t *root, rectangle2d_t query_rectangle, 
array_t *matches) {
    if (root->flag == LEAF && pointd_in_rectangle(query_rectangle, 
    root->data->coordinate)) {
        add_new_data_array(matches, root->data->footpaths);
        return;
    }

    // for each quadrant node (if they exist) check if its region overlaps with
    // the queried region "query_rectangle". if yes look into the node for all
    // matching data points

    if (root->sw_region != NULL) {
        if (rectangle_overlap(root->sw_region->region, query_rectangle)) {
            printf(" SW");
            search_qtree_rectangle(root->sw_region, query_rectangle, matches);
        }
    }
    
    if (root->nw_region != NULL) {
        if (rectangle_overlap(root->nw_region->region, query_rectangle)) {
            printf(" NW");
            search_qtree_rectangle(root->nw_region, query_rectangle, matches);
        }
    }

    if (root->ne_region != NULL) {
        if (rectangle_overlap(root->ne_region->region, query_rectangle)) {
            printf(" NE");
            search_qtree_rectangle(root->ne_region, query_rectangle, matches);
        }
    }

    if (root->se_region != NULL) {
        if (rectangle_overlap(root->se_region->region, query_rectangle)) {
            printf(" SE");
            search_qtree_rectangle(root->se_region, query_rectangle, matches);
        }
    }

    return;
}

// frees all space allocated to a quadtree node "root"
void free_qtree_node(qtree_node_t *root) {
    if (root->data != NULL) {
        free_index(root->data->footpaths);
        free(root->data);
    }
}

// frees all space allocated to a quadtree with root "root"
void free_qtree(qtree_node_t *root) {
    if (root == NULL) {
        return;
    }
  
    free_qtree(root->sw_region);
    free_qtree(root->nw_region);
    free_qtree(root->ne_region);
    free_qtree(root->se_region);
    
    free_qtree_node(root);
    free(root);
}
