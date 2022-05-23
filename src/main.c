#include "galaxy.h"

int WINDOW_WIDTH  = 900;
int WINDOW_HEIGHT = 900;

double G  = 6.674e-11; /* the gravitational constant */
double dt = 0.1 ;      /* time step */

double soomthing_length = 1e4;

int main(int argc, char *argv[]) {
    MLV_Event event = MLV_NONE;        //Évènenemt
    int mouse_x = 0, mouse_y = 0;

    Galaxy galaxy = {.nb_stars = 0, .widthOfRegion = 0, .stars = NULL};
    Quadtree *quadtree = NULL;

    int quit = 0;

    if (argc != 2) {
        printf("Usage: %s <file_of_stars>\n", argv[0]);
        return 1;
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

    /* Initialise the galaxy */
    //init_galaxy(file, &galaxy);


    /* Draw the galaxy */
    draw_galaxy(&galaxy);

    double timestamp = 0.0;
    /* Update the galaxy */
    do {
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
        update_galaxy(&galaxy, quadtree);
        draw_galaxy(&galaxy);
        
        MLV_update_window();
        
         /* increment time */
        timestamp += dt;
        //MLV_wait_milliseconds(1000);
        MLV_delay_according_to_frame_rate();
    } while (!quit);

    free_galaxy(&galaxy);

    MLV_free_window();

    return 0;
}
