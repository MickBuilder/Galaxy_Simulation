#ifndef __QTREE_H__
#define __QTREE_H__

#include <stdlib.h>
#include <stdbool.h>
#include <MLV/MLV_all.h>
#include "star.h"

#define QTREE_CAPACITY 4

typedef struct region{
    int x, y, width, height;
} Region;

typedef struct quadtree{
    Region region;
    Star *star;
    struct quadtree *nw, *ne, *sw, *se;
    Point *mass_center;
    double mass;
    bool is_leaf;
} Quadtree;

Region *create_region(int x, int y, int width, int height);

void display_region(Region region, double scale);

bool is_in_region(Region region, Star star);

bool is_far_from_star(Region region, Point *mass_center, Star star);

Quadtree *create_quadtree(Region region);

void display_quadtree(Quadtree *quadtree, double scale);

void print_quadtree(Quadtree *quadtree);

void compute_gravitational_force(Quadtree *quadtree, Star *star);

void subdivide_quadtree(Quadtree *quadtree);

void insert_star(Quadtree *quadtree, Star *star);

void free_quadtree(Quadtree *quadtree);

#endif /* __QTREE_H__ */