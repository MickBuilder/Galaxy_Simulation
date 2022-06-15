#include "galaxy.h"

void create_galaxy(FILE *file, Galaxy *galaxy) {
    int nb_stars;
    double widthOfRegion;

    fscanf(file, "%d", &nb_stars);
    fscanf(file, "%lf", &widthOfRegion);

    galaxy->nb_stars = nb_stars;
    galaxy->widthOfRegion = widthOfRegion;
    galaxy->stars = (Star**)malloc(sizeof(Star*) * nb_stars);

    for (int i = 0; i < nb_stars; i++) {
        double x, y, vx, vy, mass;
        fscanf(file, "%lf %lf %lf %lf %lf", &x, &y, &vx, &vy, &mass);

        galaxy->stars[i] = (Star*)malloc(sizeof(Star));

        Star *star = create_star(x, y, vx, vy, mass);
        galaxy->stars[i] = star;
    }
    printf("Galaxy created\n");
}

void draw_galaxy(Galaxy *galaxy) {
    for (int i = 0; i < galaxy->nb_stars; i++) {
        Star *star = galaxy->stars[i];
        draw_star(star, galaxy->widthOfRegion);
    }
}

void init_galaxy(FILE *file, Galaxy *galaxy) {
    create_galaxy(file, galaxy);
    draw_galaxy(galaxy);
}

void update_galaxy_simple(Galaxy *galaxy, Quadtree *quadtree) {
    for (int i = 0; i < galaxy->nb_stars; i++) {
        Star *star = galaxy->stars[i];
        if (star != NULL) {
            init_acceleration(&star);
            for (int j = 0; j < galaxy->nb_stars; j++) {
                if (i != j) {
                    Star *other_star = galaxy->stars[j];

                    if (other_star != NULL) {
                        if(distance(star->position, other_star->position) < MIN_DISTANCE_BETWEEN_STARS
                            && star->mass > other_star->mass) {
                            //printf("too close. Eat him!\n");
                            merge_star(&star, other_star);
                        } else {
                            update_star_acceleration(&star, other_star->position, other_star->mass);
                        }
                    } else {
                        printf("other star is null\n");
                        continue;
                    }
                }
            }
        }
    }

    for (int i = 0; i < galaxy->nb_stars; i++) {
        Star *star = galaxy->stars[i];
        if (star != NULL) {
            update_star_velocity(&star);
            update_star_position(&star);
        } else {
            printf("star is null\n");
            continue;
        }
        /*update_star_velocity(&star);
        update_star_position(&star);*/
    }

    /*for (int i = 0; i < galaxy->nb_stars; i++) {
        Star *star = galaxy->stars[i];
        for (int j = 0; j < galaxy->nb_stars; j++) {
            if (i != j) {
                Star *other_star = galaxy->stars[j];
                if (star != NULL && other_star != NULL
                    && distance(star->position, other_star->position) < MIN_DISTANCE_BETWEEN_STARS
                    && star->mass > other_star->mass) {
                    printf("to close. Eat him!\n");
                    merge_star(&star, other_star);
                }
            }
        }
    }*/
}

void update_galaxy_with_quadtree(Galaxy *galaxy, Quadtree *quadtree, int show) {
    Region *region = create_region(-galaxy->widthOfRegion/2, -galaxy->widthOfRegion/2, galaxy->widthOfRegion, galaxy->widthOfRegion);
    quadtree = create_quadtree(*region);

    for (int i = 0; i < galaxy->nb_stars; i++) {
        Star *star = galaxy->stars[i];
        insert_star(quadtree, star);
    }

    for (int i = 0; i < galaxy->nb_stars; i++) {
        Star *star = galaxy->stars[i];
        init_acceleration(&star);
        compute_gravitational_acceleration(quadtree, star);
    }
    
    if(show) {
        display_quadtree(quadtree, galaxy->widthOfRegion);
    }

    for (int i = 0; i < galaxy->nb_stars; i++) {
        Star *star = galaxy->stars[i];
        update_star_velocity(&star);
        update_star_position(&star);
    }

    free_quadtree(quadtree);
}

void free_galaxy(Galaxy *galaxy) {
    for (int i = 0; i < galaxy->nb_stars; i++) {
        Star *star = galaxy->stars[i];
        free_star(star);
    }
}
