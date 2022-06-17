#ifndef __STAR_BODY_H__
#define __STAR_BODY_H__

#include <stdio.h>
#include <math.h>
#include <MLV/MLV_all.h>

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern double dt;
extern double G;
extern double soomthing_length;

#define DEFAULT_MASS    1e18
#define DEFAULT_RADIUS  1
#define DEFALUT_COLOR   MLV_COLOR_WHITE
#define RANDOM_COLOR    MLV_rgba(rand()%256, rand()%256, rand()%256, 255)

typedef struct point {
    double x;
    double y;
} Point;

typedef struct star {
    Point *position;
    Point *velocity;
    Point *acceleration;
    double mass;
    double radius;
    MLV_Color color;
} Star;

typedef struct _body {
    double px;   /* x position */
    double py;   /* y position */
    double vx;   /* x velocity */
    double vy;   /* y velocity */
    double fx;   /* x force */
    double fy;   /* y force */
    double mass; /* mass */
} body;

Point *create_point(double x, double y);

double distance(Point *p1, Point *p2);

Star *create_star(double x, double y, double vx, double vy, double mass);

void draw_star(Star *star, double scale);

void init_acceleration(Star **star);

void update_star_acceleration(Star **star, Point *other_star_position, double other_star_mass);

void update_star_velocity(Star **star);

void update_star_position(Star **star);

void update_star(Star **star, Star *other_star);

void merge_star(Star **star, Star **other_star);

void free_star(Star *star);

#endif /* __STARS_BODY_H__ */