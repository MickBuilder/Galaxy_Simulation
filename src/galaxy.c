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

    //display_quadtree(galaxy->quadtree, galaxy->widthOfRegion);
}

void init_galaxy(FILE *file, Galaxy *galaxy) {
    create_galaxy(file, galaxy);
    draw_galaxy(galaxy);
}

void update_galaxy(Galaxy *galaxy, Quadtree *quadtree) {
    /*for (int i = 0; i < galaxy->nb_stars; i++) {
        Star *star = galaxy->stars[i];
        init_acceleration(&star);
        for (int j = 0; j < galaxy->nb_stars; j++) {
            if (i != j) {
                Star *other_star = galaxy->stars[j];
                update_star_acceleration(&star, other_star->position, other_star->mass);
            }
        }
    }

    for (int i = 0; i < galaxy->nb_stars; i++) {
        Star *star = galaxy->stars[i];
        update_star_velocity(&star);
        update_star_position(&star);
    }*/

    Region *region = create_region(-galaxy->widthOfRegion/2, -galaxy->widthOfRegion/2, galaxy->widthOfRegion, galaxy->widthOfRegion);
    //MLV_draw_rectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, MLV_COLOR_RED);
    quadtree = create_quadtree(*region);

    for (int i = 0; i < galaxy->nb_stars; i++) {
        Star *star = galaxy->stars[i];
        insert_star_2(quadtree, star);
    }

    for (int i = 0; i < galaxy->nb_stars; i++) {
        Star *star = galaxy->stars[i];
        init_acceleration(&star);
        //compute_gravitational_force(quadtree, star);
        compute_gravitational_acceleration(quadtree, star);
    }

    //display_quadtree(quadtree, galaxy->widthOfRegion);

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
