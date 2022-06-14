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
        printf("Usage: %s ./data/stars_....txt\n", argv[0]);
        /* ./exec.exe ./data/stars_...txt --quad --show */
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
        update_galaxy(&galaxy, quadtree);
        MLV_draw_text(10, 25, "FPS: %d", 
            MLV_COLOR_WHITE, fps
        );
        draw_galaxy(&galaxy);
        
        MLV_update_window();
        
         /* increment time */
        timestamp += dt;
        //MLV_wait_milliseconds(1000);
        MLV_delay_according_to_frame_rate();
        frame_counter++;
    } while (!quit);

    free_galaxy(&galaxy);

    MLV_free_window();

    return 0;
}
