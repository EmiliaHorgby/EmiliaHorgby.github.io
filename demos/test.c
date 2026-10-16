#include "./olive.c/demos/vc.c"

#define WIDTH 960
#define HEIGHT 720
#define BACKGROUND_COLOR 0xFF181818
#define CIRCLE_RADIUS 100
#define CIRCLE_COLOR 0x99AA2020

static uint32_t pixels[WIDTH*HEIGHT];
static float triangle_angle = 0;
static float circle_x = WIDTH/2;
static float circle_y = HEIGHT/2;
static float circle_dx = 100;
static float circle_dy = 100;

float sqrtf(float x);
float atan2f(float y, float x);
float sinf(float x);
float cosf(float x);

#define PI 3.14159265359

static inline void rotate_point(float *x, float *y)
{
    float dx = *x - WIDTH/2;
    float dy = *y - HEIGHT/2;
    float mag = sqrtf(dx*dx + dy*dy);
    float dir = atan2f(dy, dx) + triangle_angle;
    *x = cosf(dir)*mag + WIDTH/2;
    *y = sinf(dir)*mag + HEIGHT/2;
}


Olivec_Canvas vc_render(float dt)
{
    
    
    Olivec_Canvas oc = olivec_canvas(pixels, WIDTH, HEIGHT, WIDTH);

    olivec_fill(oc, BACKGROUND_COLOR);

    // Circle
    
    olivec_circle(oc, 100., 100., CIRCLE_RADIUS, CIRCLE_COLOR);


    return oc;
}
