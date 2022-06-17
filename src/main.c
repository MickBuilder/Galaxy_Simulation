#include <time.h>
#include "galaxy.h"

int WINDOW_WIDTH  = 900;
int WINDOW_HEIGHT = 900;

double G  = 6.674e-11; /* the gravitational constant */
double dt = 0.1 ;      /* time step */

double soomthing_length = 1e4;

typedef enum version {SIMPLE, QUADTREE} version;

void display_help(const char* str) {
    printf("Usage: %s ./data/stars_....txt\n", str);
    printf("\t--quad\tversion de la simulation utilisant un quadtree\n");
    printf("\t--show\tvisualiser le quadtree à l'aide des carrés dans la galaxy [! Fonctionne que si le quadtree est utilisé.]\n");

    return;
}

void display_stats(version vers, int nb_tour, double avg_simulation) {
    printf("Algo : %s\n", (vers == QUADTREE) ? "Quadtree" : "Iterative");
    printf("Number of tour: %d\n", nb_tour);
    printf("Average simulation time: %lf secondes\n", avg_simulation);
}

int main(int argc, char *argv[]) {
    MLV_Event event = MLV_NONE;        //Évènenemt
    int mouse_x = 0, mouse_y = 0;

    Galaxy galaxy = {.nb_stars = 0, .widthOfRegion = 0, .stars = NULL};
    Quadtree *quadtree = NULL;

    int quit = 0, nb_tour = 0, show_quadtree = 0;
    double simulation_time = 0.0;
    version vers = SIMPLE;

    if (argc < 2 || argc > 4) {
        //printf("Usage: %s ./data/stars_....txt\n", argv[0]);
        display_help(argv[0]);
        /* ./exec.exe ./data/stars_...txt --quad --show */
        return 1;
    }

    for(int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--quad") == 0) {
            vers = QUADTREE;
        } else if(strcmp(argv[i], "--show") == 0) {
            if(vers != QUADTREE) {
                display_help(argv[0]); 
                return 0; 
            } 
            show_quadtree = 1;
        } else {
            display_help(argv[0]);
            return 1;
        }
    }
    
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Error: can't open file %s\n", argv[1]);
        return 1;
    }

    create_galaxy(file, &galaxy);

    fclose(file);

    /* Open the window */
    MLV_create_window("Galaxy Simulation", NULL, WINDOW_WIDTH, WINDOW_HEIGHT);

    double timestamp = 0.0;
    int old_time = MLV_get_time();
    int new_time = MLV_get_time();
    int fps = 0, frame_counter = 0;
    /* Update the galaxy */
    do {
        // to compute the fps
/*         int new_time = MLV_get_time();
        double time_second = (new_time - old_time) / 1000.0;
        old_time = new_time; */
        if(new_time - old_time >= 1000){
            old_time = MLV_get_time();
            fps = frame_counter;
            frame_counter = 0;
        }
        new_time = MLV_get_time();

        event = MLV_get_event(NULL, NULL, NULL, NULL, NULL, &mouse_x, &mouse_y, NULL, NULL);
        switch (event) {
            case MLV_MOUSE_BUTTON:
                quit = 1;
                break;
            case MLV_KEY:
                quit = 1;
                break;
            default:
                break;
        }

        MLV_draw_filled_rectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, MLV_COLOR_BLACK);

        if(vers == SIMPLE) {
            time_t start = time(NULL);
            update_galaxy_simple(&galaxy, quadtree);
            time_t end = time(NULL);

            simulation_time += difftime(end, start);
        } else {
            time_t start = time(NULL);
            update_galaxy_with_quadtree(&galaxy, quadtree, show_quadtree);
            time_t end = time(NULL);

            simulation_time += difftime(end, start);
        }

        MLV_draw_text(10, 25, "FPS: %d", 
            MLV_COLOR_WHITE, fps
        );
        draw_galaxy(&galaxy);
        
        MLV_update_window();
        
         /* increment time */
        nb_tour++;
        timestamp += dt;
        frame_counter++;

        if(nb_tour == 150) {
            quit = 1;
        }
        //MLV_wait_milliseconds(1000);
        MLV_delay_according_to_frame_rate();
    } while (!quit);

    free_galaxy(&galaxy);
    MLV_free_window();

    display_stats(vers, nb_tour, simulation_time/nb_tour);

    return 0;
}
