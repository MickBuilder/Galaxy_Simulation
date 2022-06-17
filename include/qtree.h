#ifndef __QTREE_H__
#define __QTREE_H__

#include <stdlib.h>
#include <stdbool.h>
#include <MLV/MLV_all.h>
#include "star.h"

typedef struct region{
    double x, y, width, height;
} Region;

typedef struct quadtree{
    Region region;
    Star *star;
    struct quadtree *nw, *ne, *sw, *se;
    Point *mass_center;
    double mass;
    bool is_leaf;
} Quadtree;

/**
 * It creates a new region
 * 
 * @param x The x coordinate of the top left corner of the region.
 * @param y The y coordinate of the top left corner of the region.
 * @param width The width of the region.
 * @param height The height of the region.
 * 
 * @return A pointer to a region.
 */
Region *create_region(double x, double y, double width, double height);

/**
 * It takes a region and a width of the region, and displays the region on the screen
 * 
 * @param region the region to display
 * @param widthOfRegion the width of the region we're currently looking at.
 */
void display_region(Region region, double scale);

/**
 * It returns true if the star is in the region, and false otherwise
 * 
 * @param region The region to check if the star is in.
 * @param star The star to check if it's in the region.
 * 
 * @return A boolean value.
 */
bool is_in_region(Region region, Star star);

/**
 * If the distance between the star and the mass center of the region is greater than half the width of
 * the region, then the star is far from the region.
 * 
 * @param region the region of the image we're looking at
 * @param mass_center the center of mass of the region
 * @param star the star we're trying to find
 * 
 * @return A boolean value.
 */
bool is_far_from_star(Region region, Point *mass_center, Star star);

/**
 * It creates a new quadtree with the given region and sets all of its fields to their default values
 * 
 * @param region The region of space that this quadtree represents.
 * 
 * @return A pointer to a Quadtree struct.
 */
Quadtree *create_quadtree(Region region);

/**
 * It displays the region of the quadtree, and then recursively displays the four quadrants of the
 * quadtree
 * 
 * @param quadtree The quadtree to display.
 * @param scale the scale of the display.
 * 
 * @return void 
 */
void display_quadtree(Quadtree *quadtree, double scale);

void print_quadtree(Quadtree *quadtree);

/**
 * If the quadtree is a leaf and the star is not the star we're computing the acceleration for, then
 * add the acceleration. If the quadtree is not a leaf, then check if the quadtree is far enough from
 * the star to be considered a single point mass. If it is, then add the acceleration. If it isn't,
 * then recursively call the function on the quadtree's children
 * 
 * @param quadtree The quadtree to compute the acceleration for
 * @param star the star we're computing the acceleration for
 * 
 * @return the gravitational acceleration of the star.
 */
void compute_gravitational_acceleration(Quadtree *quadtree, Star *star);

/**
 * It takes a quadtree and creates four new quadtrees, one for each quadrant of the original quadtree
 * 
 * @param quadtree The quadtree to subdivide.
 */
void subdivide_quadtree(Quadtree *quadtree);

/**
 * If the star is in the region, then if the quadtree is a leaf, then if the quadtree has no star, then
 * insert the star, else subdivide the quadtree and reinsert the old star and the new star, else insert
 * the star in all four quadrants
 * 
 * @param quadtree The quadtree to insert the star into.
 * @param star The star to insert into the quadtree.
 * 
 * @return nothing
 */
void insert_star(Quadtree *quadtree, Star *star);

/**
 * @brief Free the given quadtree
 * 
 * @param quadtree 
 */
void free_quadtree(Quadtree *quadtree);

#endif /* __QTREE_H__ */