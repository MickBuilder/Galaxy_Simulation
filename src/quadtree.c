#include "qtree.h"

Region *create_region(double x, double y, double width, double height) {
    Region *region = malloc(sizeof(Region));
    region->x = x;
    region->y = y;
    region->width = width;
    region->height = height;

    return region;
}

void display_region(Region region, double widthOfRegion) {
    double window_x = WINDOW_WIDTH * (0.5 + (region.x / widthOfRegion));
    double window_y = WINDOW_HEIGHT * (0.5 + (region.y / widthOfRegion));
    double window_width = WINDOW_WIDTH * ((region.width / widthOfRegion));
    double window_height = WINDOW_HEIGHT * ((region.height / widthOfRegion));

    MLV_draw_rectangle(window_x, window_y, window_width, window_height, MLV_COLOR_RED);
}

bool is_in_region(Region region, Star star) {
    return (star.position->x >= region.x && star.position->x <= region.x + region.width &&
            star.position->y >= region.y && star.position->y <= region.y + region.height);
}

bool is_far_from_star(Region region, Point *mass_center, Star star) {
    double dist = distance(star.position, mass_center);
    double region_width = region.width;

    return region_width < dist / 2;
}

Quadtree *create_quadtree(Region region) {
    Quadtree *quadtree = malloc(sizeof(Quadtree));
    quadtree->region = region;
    quadtree->star = NULL;
    quadtree->nw = NULL;
    quadtree->ne = NULL;
    quadtree->sw = NULL;
    quadtree->se = NULL;
    quadtree->mass_center = NULL;
    quadtree->mass = 0;
    quadtree->is_leaf = true;

    return quadtree;
}

void display_quadtree(Quadtree *quadtree, double scale) {
    if (quadtree == NULL) {
        return;
    }

    display_region(quadtree->region, scale);

    /*if (quadtree->star != NULL) {
        draw_star(quadtree->star, scale);
    }*/

    display_quadtree(quadtree->nw, scale);
    display_quadtree(quadtree->ne, scale);
    display_quadtree(quadtree->sw, scale);
    display_quadtree(quadtree->se, scale);
}

void print_quadtree(Quadtree *quadtree) {
    if (quadtree == NULL) {
        return;
    }

    printf("Region: %f %f %f %f\n", quadtree->region.x, quadtree->region.y, quadtree->region.width, quadtree->region.height);
    if (quadtree->star != NULL) {
        printf("Star: %f %f %f\n", quadtree->star->position->x, quadtree->star->position->y, quadtree->star->mass);
    }
    if (quadtree->mass_center != NULL) {
        printf("Mass center: %f %f\n", quadtree->mass_center->x, quadtree->mass_center->y);
    }
    printf("Mass: %f\n", quadtree->mass);

    print_quadtree(quadtree->nw);
    print_quadtree(quadtree->ne);
    print_quadtree(quadtree->sw);
    print_quadtree(quadtree->se);
}

void compute_gravitational_acceleration(Quadtree *quadtree, Star *star) {
    if (quadtree == NULL) {
        return;
    }

    if (quadtree->star != NULL && quadtree->is_leaf) {
        //printf("It's a leaf and different so add force\n");
        if(quadtree->star != star) {
            update_star_acceleration(&star, quadtree->star->position, quadtree->star->mass);
            return;
        }
        //display_region(quadtree->region, 2.83800e+06);
    } else if (!quadtree->is_leaf){
        //printf("It's a node so check the distance and add force\n");
        if (is_far_from_star(quadtree->region, quadtree->mass_center, *star)) {
            update_star_acceleration(&star, quadtree->mass_center, quadtree->mass);
            //display_region(quadtree->region, 2.83800e+06);
            return;
        } else {
            compute_gravitational_acceleration(quadtree->nw, star);
            compute_gravitational_acceleration(quadtree->ne, star);
            compute_gravitational_acceleration(quadtree->sw, star);
            compute_gravitational_acceleration(quadtree->se, star);
        }
    }
    //display_region(quadtree->region, 2.83800e+06);
}

void subdivide_quadtree(Quadtree *quadtree) {
    int x = quadtree->region.x;
    int y = quadtree->region.y;
    int width = quadtree->region.width;
    int height = quadtree->region.height;

    //printf("Region: %d, %d, %d, %d\n", x, y, width, height);

    Region* nw_region = create_region(x, y, width / 2, height / 2);
    Region* ne_region = create_region(x + width / 2, y, width / 2, height / 2);
    Region* sw_region = create_region(x, y + height / 2, width / 2, height / 2);
    Region* se_region = create_region(x + width / 2, y + height / 2, width / 2, height / 2);

    quadtree->nw = create_quadtree(*nw_region);
    quadtree->ne = create_quadtree(*ne_region);
    quadtree->sw = create_quadtree(*sw_region);
    quadtree->se = create_quadtree(*se_region);

    free(nw_region);
    free(ne_region);
    free(sw_region);
    free(se_region);
}

static void insert_star_in_children(Quadtree *quadtree, Star *star) {
    if(is_in_region(quadtree->nw->region, *star)) {
        insert_star(quadtree->nw, star);
    } else if (is_in_region(quadtree->ne->region, *star)) {
        insert_star(quadtree->ne, star);
    } else if (is_in_region(quadtree->se->region, *star)) {
        insert_star(quadtree->se, star);
    } else if (is_in_region(quadtree->sw->region, *star)) {
        insert_star(quadtree->sw, star);
    }
}

void insert_star(Quadtree *quadtree, Star *star) {
    /*if (!is_in_region(quadtree->region, *star)) {
        return;
    }*/

    if (!quadtree->is_leaf) {
        double new_mass = quadtree->mass + star->mass;
        double new_center_x = (quadtree->mass_center->x * quadtree->mass + star->position->x * star->mass) / new_mass;
        double new_center_y = (quadtree->mass_center->y * quadtree->mass + star->position->y * star->mass) / new_mass;

        quadtree->mass = new_mass;

        /* Update mass center */
        quadtree->mass_center->x = new_center_x;
        quadtree->mass_center->y = new_center_y;

        /* Insert new star */
        insert_star(quadtree->nw, star);
        insert_star(quadtree->ne, star);
        insert_star(quadtree->sw, star);
        insert_star(quadtree->se, star);
        return;
    }

    if (quadtree->star == NULL) {
        quadtree->star = star;
    } else {
        subdivide_quadtree(quadtree);
        quadtree->is_leaf = false;

        quadtree->mass_center = create_point(0.0, 0.0);
        quadtree->mass = 0;

        Star *old_star = quadtree->star;
        quadtree->star = NULL;
        /* Reinsert old star */
        /*insert_star(quadtree->nw, old_star);
        insert_star(quadtree->ne, old_star);
        insert_star(quadtree->sw, old_star);
        insert_star(quadtree->se, old_star);*/
        insert_star_in_children(quadtree, old_star);


        /* Insert new star */
        /*insert_star(quadtree->nw, star);
        insert_star(quadtree->ne, star);
        insert_star(quadtree->sw, star);
        insert_star(quadtree->se, star);*/
        insert_star_in_children(quadtree, star);
    }

}

void free_quadtree(Quadtree *quadtree) {
    if (quadtree == NULL) {
        return;
    }

    free(quadtree->mass_center);

    free_quadtree(quadtree->nw);
    free_quadtree(quadtree->ne);
    free_quadtree(quadtree->sw);
    free_quadtree(quadtree->se);

    free(quadtree);
}
