#include <stdio.h>
#include <math.h>
#include <MLV/MLV_all.h>

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400
#define WIDTH_OF_REGION 4e4

#define G  6.674e-11 /* the gravitational constant */
#define dt 0.1       /* time step */

typedef struct _body {
    double px;   /* x position */
    double py;   /* y position */
    double vx;   /* x velocity */
    double vy;   /* y velocity */
    double fx;   /* x force */
    double fy;   /* y force */
    double mass; /* mass */
} body;

void draw_body(body *B) {

    int window_x = WINDOW_WIDTH*(0.5+0.5*(B->px/WIDTH_OF_REGION));
    int window_y = WINDOW_HEIGHT*(0.5+0.5*(B->py/WIDTH_OF_REGION));

    MLV_draw_filled_circle(window_x, window_y, 10, MLV_COLOR_WHITE);

}

int main(int argc, char *argv[]) {

    /* Open the window */
    MLV_create_window("Window", "Window", WINDOW_WIDTH, WINDOW_HEIGHT);

    /* Initialise two bodies */
    body B1 = {.px = 1e4, .py = -1e4, .vx = 2.5e3, .vy = 0.0, .mass = 4e21};
    body B2 = {.px = -1e4, .py = 1e4, .vx = -2.5e3, .vy = 0.0, .mass = 4e21};

    double t = 0.0;
    while (1) {
        double dx, dy, dist;
        
        /* compute the gravitational forces */

        /* compute the vector from B1 to B2 and the distance */
        dx = B2.px-B1.px;
        dy = B2.py-B1.py;
        dist = sqrt(dx*dx+dy*dy); /* distance between B1 and B2 */

        /* compute the gravitational force acting on B1 from B2 */
        B1.fx = (G*B1.mass*B2.mass/(dist*dist))*(dx/dist); 
        B1.fy = (G*B1.mass*B2.mass/(dist*dist))*(dy/dist); 

        /* compute the vector from B2 to B1 and the distance */
        dx = B1.px-B2.px;
        dy = B1.py-B2.py;
        dist = sqrt(dx*dx+dy*dy);

        /* compute the gravitational force acting on B2 from B1 */
        B2.fx = (G*B1.mass*B2.mass/(dist*dist))*(dx/dist); 
        B2.fy = (G*B1.mass*B2.mass/(dist*dist))*(dy/dist); 

        /* compute new velocities and positions */

        /* compute B1's new velocity */
        B1.vx += dt*B1.fx/B1.mass;
        B1.vy += dt*B1.fy/B1.mass;
        /* compute B1's new position */
        B1.px += dt*B1.vx;
        B1.py += dt*B1.vy;

        /* compute B2's new velocity */
        B2.vx += dt*B2.fx/B2.mass;
        B2.vy += dt*B2.fy/B2.mass;
        /* compute B2's new position */
        B2.px += dt*B2.vx;
        B2.py += dt*B2.vy;

        /* draw */
        MLV_draw_filled_rectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, MLV_COLOR_BLACK);
        draw_body(&B1);
        draw_body(&B2);
        MLV_update_window();

        /* increment time */
        t += dt;
        MLV_wait_milliseconds(20);
    }

    /* Close the window */
    MLV_free_window();

    return 0;
}
