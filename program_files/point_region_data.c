/*----------------------------------------------------------------------------
Project: dict3, dict4
point_region_data.c : the implementation of the module dealing with 2D point 
region data
----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

#include "point_region_data.h"

enum quadrant {SW = 0, NW = 1, NE = 2, SE = 3};

// returns 1 if "point_1" and "point_2" are the same point and 0 otherwise
// (double valued points)
int is_same_point(pointd_t point_1, pointd_t point_2) {
    return ((point_1.x == point_2.x) && (point_1.y == point_2.y));
}

// returns midpoint between "point_1" and "point_2"
// (long double valued points)
pointld_t midpoint(pointld_t point_1, pointld_t point_2) {
    pointld_t midpoint;
    midpoint.x = (point_1.x + point_2.x)/2;
    midpoint.y = (point_1.y + point_2.y)/2;

    return midpoint;
}

// given a rectangle's bottom-left point "sw" and top-right point "ne", returns
// a rectangle with these points (long double valued points)
rectangle2d_t make_rectangle(pointld_t sw, pointld_t ne) {
    rectangle2d_t rectangle;

    rectangle.sw_point = sw;
    rectangle.ne_point = ne;

    rectangle.nw_point.x = rectangle.sw_point.x;
    rectangle.nw_point.y = rectangle.ne_point.y;

    rectangle.se_point.x = rectangle.ne_point.x;
    rectangle.se_point.y = rectangle.sw_point.y;

    return rectangle;
}

// returns 1 if a "point" lies within the "rectangle" and 0 otherwise. points
// are within a rectangle if they are on the lower or right rectangle edges,
// but not if they are on the top or left edge (long double valued points)
int pointld_in_rectangle(rectangle2d_t rectangle, pointld_t point) {
    return (rectangle.sw_point.x < point.x && point.x <= rectangle.ne_point.x
    && rectangle.sw_point.y <= point.y && point.y < rectangle.ne_point.y);
}

// returns 1 if a "point" lies within the "rectangle" and 0 otherwise. points
// are within a rectangle if they are on the lower or right rectangle edges,
// but not if they are on the top or left edge (double valued points)
int pointd_in_rectangle(rectangle2d_t rectangle, pointd_t point) {
    return (rectangle.sw_point.x < point.x && point.x <= rectangle.ne_point.x
    && rectangle.sw_point.y <= point.y && point.y < rectangle.ne_point.y);
}

// returns 1 if "rectangle_1" and "rectangle_2" overlap and 0 otherwise. we
// know two rectangles overlap if any of their corner points are in the region
// bounded by the other rectangle
int rectangle_overlap(rectangle2d_t rectangle_1, rectangle2d_t rectangle_2) {
    if (pointld_in_rectangle(rectangle_1, rectangle_2.sw_point) ||
    pointld_in_rectangle(rectangle_1, rectangle_2.nw_point) ||
    pointld_in_rectangle(rectangle_1, rectangle_2.ne_point) ||
    pointld_in_rectangle(rectangle_1, rectangle_2.se_point)) {
        return 1;
    } 
    
    else if (pointld_in_rectangle(rectangle_2, rectangle_1.sw_point) ||
    pointld_in_rectangle(rectangle_2, rectangle_1.nw_point) ||
    pointld_in_rectangle(rectangle_2, rectangle_1.ne_point) ||
    pointld_in_rectangle(rectangle_2, rectangle_1.se_point)) {
        return 1;
    } 
    
    else {
        return 0;
    }
}

// returns the quadrant of the "rectangle" that "point" lies in as an integer
int determine_quadrant(rectangle2d_t rectangle, pointd_t point) {
    pointld_t rectangle_centre = midpoint(rectangle.sw_point, 
    rectangle.ne_point);

    if (point.x <= rectangle_centre.x && point.y >= rectangle_centre.y) {
        return NW;
    } else if (point.x <= rectangle_centre.x && point.y < rectangle_centre.y) {
        return SW;
    } else if (point.x > rectangle_centre.x && point.y >= rectangle_centre.y) {
        return NE;
    } else {
        return SE;
    }
}

// returns the region bounded by the quadrant with "quadrant_code" of a 
// "rectangle" as a rectangle itself
rectangle2d_t get_quadrant(rectangle2d_t rectangle, int quadrant_code) {
    pointld_t rectangle_centre = midpoint(rectangle.sw_point, 
    rectangle.ne_point);

    rectangle2d_t quadrant;
    pointld_t quadrant_sw_point, quadrant_ne_point;

    switch (quadrant_code) {
        case SW:
            quadrant = make_rectangle(rectangle.sw_point, rectangle_centre);
            break;
        case NE:
            quadrant = make_rectangle(rectangle_centre, rectangle.ne_point);
            break;
        case NW:
            quadrant_sw_point = midpoint(rectangle.sw_point, 
            rectangle.nw_point);
            quadrant_ne_point = midpoint(rectangle.ne_point, 
            rectangle.nw_point);
            quadrant = make_rectangle(quadrant_sw_point, quadrant_ne_point);
            break;
        case SE:
            quadrant_sw_point = midpoint(rectangle.sw_point, 
            rectangle.se_point);
            quadrant_ne_point = midpoint(rectangle.se_point, 
            rectangle.ne_point);
            quadrant = make_rectangle(quadrant_sw_point, quadrant_ne_point);
            break;
    }

    return quadrant;
}
