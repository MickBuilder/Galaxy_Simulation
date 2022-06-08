#ifndef __GALAXY_H__
#define __GALAXY_H__

#include <MLV/MLV_all.h>  
#include "qtree.h"
#include "star.h"

#define MIN_DISTANCE_BETWEEN_STARS 1e5

typedef struct galaxy_{
    int nb_stars;
    double widthOfRegion;
    Star **stars;
} Galaxy;

void create_galaxy(FILE *file, Galaxy *galaxy);

void init_galaxy(FILE *file, Galaxy *galaxy);

void draw_galaxy(Galaxy *galaxy);

void update_galaxy(Galaxy *galaxy, Quadtree *quadtree);

void free_galaxy(Galaxy *galaxy);

#endif /* __GALAXY_H__ */