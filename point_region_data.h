/*----------------------------------------------------------------------------
Project: dict3, dict4
point_region_data.h : the interface of the module dealing with 2D point region
data
----------------------------------------------------------------------------*/

#ifndef _POINT_REGION_DATA_H_
#define _POINT_REGION_DATA_H_

typedef struct pointd pointd_t;
typedef struct pointld pointld_t;
typedef struct rectangle2d rectangle2d_t;

// a point with coordinates represented with double values
struct pointd {
    double x; // x coordinate of a 2d point (double)
    double y; // y coordinate of a 2d point (double)
};

// a point with coordinates represented with long double values
struct pointld {
    long double x; // x coordinate of a 2d point (long double)
    long double y; // y coordinate of a 2d point (long double)
};

// 2d rectangle with 4 corner points
struct rectangle2d {
    pointld_t sw_point; // coordinate of rectangle's SW corner
    pointld_t nw_point; // coordinate of rectangle's NW corner
    pointld_t ne_point; // coordinate of rectangle's NE corner
    pointld_t se_point; // coordinate of rectangle's SE corner
};

/*---------------------------function definitions---------------------------*/

// returns 1 if "point_1" and "point_2" are the same point and 0 otherwise
// (double valued points)
int is_same_point(pointd_t point_1, pointd_t point_2);

// returns midpoint between "point_1" and "point_2"
// (long double valued points)
pointld_t midpoint(pointld_t point_1, pointld_t point_2);

// given a rectangle's bottom-left point "sw" and top-right point "ne", returns
// a rectangle with these points (long double valued points)
rectangle2d_t make_rectangle(pointld_t sw, pointld_t ne);

// returns 1 if a "point" lies within the "rectangle" and 0 otherwise. points
// are within a rectangle if they are on the lower or right rectangle edges,
// but not if they are on the top or left edge (long double valued points)
int pointld_in_rectangle(rectangle2d_t rectangle, pointld_t point);

// returns 1 if a "point" lies within the "rectangle" and 0 otherwise. points
// are within a rectangle if they are on the lower or right rectangle edges,
// but not if they are on the top or left edge (double valued points)
int pointd_in_rectangle(rectangle2d_t rectangle, pointd_t point);

// returns 1 if "rectangle_1" and "rectangle_2" overlap and 0 otherwise. we
// know two rectangles overlap if any of their corner points are in the region
// bounded by the other rectangle
int rectangle_overlap(rectangle2d_t rectangle_1, rectangle2d_t rectangle_2);

// returns the quadrant of the "rectangle" that "point" lies in as an integer
int determine_quadrant(rectangle2d_t rectangle, pointd_t point);

// returns the region bounded by the quadrant with "quadrant_code" of a 
// "rectangle" as a rectangle itself
rectangle2d_t get_quadrant(rectangle2d_t rectangle, int quadrant);

#endif
