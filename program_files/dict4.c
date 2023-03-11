/* dict4.c
*
* Program created by: Victoria Halim (vhalim@student.unimelb.edu.au)
* Created September 2022
*
* Footpaths dataset obtained from https://data.melbourne.vic.gov.au/ and
processed by COMP20003 teaching team
*
* This program creates a PR quadtree of footpath data and outputs all footpath
records which are located within input queries of rectangular search regions.
*  
* The program takes seven arguments (excluding the program name) consisting of:
1 Stage number -> must be 4 to for this program to successfully run
2 CSV file of footpath data -> where footpath data will be read from
3 Output file -> where all matching footpath records will be printed out to.
4, 5 longitude, latitude co-ordinate pair of the bottom-left corner of the root
node area (respectively)
6, 7 longitude, latitude co-ordinate pair of the top-right corner of the root
node area (respectively)
*
* Input and output redirection:
1. Redirect stdin to the file containing queries so they can be read by the
program
2. Redirect stdout to a text file to view the order of quadrants that had to
be traversed to find matches for each query
* 
* General arguments to run program:
./dict4 4 <.csv data file> <output file> <bottom left longitude>
<bottom left latitude> <top right longitude> <top right latitude> 
< <stdin file> > <stdout file>
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "data.h"
#include "point_region_data.h"
#include "data_dictionary.h"
#include "data_index.h"
#include "quadtree.h"

int main(int argc, char **argv) {
    assert(argc == 8);
    int stage_number = atoi(argv[1]);
    assert(stage_number == 4);
    FILE *data_file = fopen(argv[2], "r");
    assert(data_file);
    FILE *out_file = fopen(argv[3], "w");
    assert(out_file);
    
    // read in and define the rectangular region all data points will be
    // bounded by
    pointld_t region_boundary_sw, region_boundary_ne;
    region_boundary_sw.x = strtold(argv[4], NULL);
    region_boundary_sw.y = strtold(argv[5], NULL);
    region_boundary_ne.x = strtold(argv[6], NULL);
    region_boundary_ne.y = strtold(argv[7], NULL);
    rectangle2d_t region_boundary = make_rectangle(region_boundary_sw, 
    region_boundary_ne);
    
    linked_list_t *footpath_dict = read_footpaths(data_file);
    qtree_node_t *root = build_qtree(region_boundary, footpath_dict); 

    // for reading in queries
    pointld_t query_sw;
    pointld_t query_ne;
    rectangle2d_t query_rectangle;
    char query[MAX_STRING_LEN + 1];
    
    // find all footpath records with a start or end point which are within
    // the query region
    while (scanf("%[^\n]\n", query) != EOF) {
        array_t *matches = make_new_array();
        assert(matches);

        fprintf(out_file, "%s\n", query);
        printf("%s -->", query);

        sscanf(query, "%Lf %Lf %Lf %Lf\n", &query_sw.x, &query_sw.y, 
        &query_ne.x, &query_ne.y);

        query_rectangle = make_rectangle(query_sw, query_ne);
        search_qtree_rectangle(root, query_rectangle, matches);
        print_footpaths(matches, out_file);
        free_index(matches);

        printf("\n");
    }

    fclose(data_file);
    fclose(out_file);

    free_qtree(root);
    free_footpath_dict(footpath_dict);
    
    return 0;
}
