#include "../olive.c/demos/vc.c"
#include <stdint.h>

#define WIDTH 960
#define HEIGHT 720
#define BACKGROUND_COLOR 0xFF181818
#define CIRCLE_RADIUS 100

static uint32_t pixels[WIDTH*HEIGHT];


static float xs[] = {100, 300, 500};
static float ys[] = {100, 300, 500};
static float dxs[] = {10, 8, 5};
static float dys[] = {7, 10, 6};
static uint32_t colors[] = {0xff15638f, 0xff8f1915, 0xff51158f};

float sqrtf(float x);
float atan2f(float y, float x);
float sinf(float x);
float cosf(float x);
float acosf(float x);

#define PI 3.14159265359

float sqrf(float x) {
    return x*x;
}

float correct_d(float x, float dx, float dt, float border){
    float perimeter = x + dx*dt;
    int cond = (perimeter - CIRCLE_RADIUS < 0 || perimeter + CIRCLE_RADIUS >= border);
    return (1-2*cond) * dx;
}

void wall_bounce(int i, float dt){
    dxs[i] = correct_d(xs[i], dxs[i], dt, WIDTH);
    xs[i] = xs[i] + dxs[i];
    dys[i] = correct_d(ys[i], dys[i], dt, HEIGHT);
    ys[i] = ys[i] + dys[i];
}

void swap(float *a, float *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

Olivec_Canvas vc_render(float dt)
{
    Olivec_Canvas oc = olivec_canvas(pixels, WIDTH, HEIGHT, WIDTH);

    olivec_fill(oc, BACKGROUND_COLOR);

    
    // Circle
    int n = *(&xs + 1) - xs;
    for (int i = 0; i < n; i++) {
        wall_bounce(i, dt);
        
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            float distance = sqrtf(sqrf(xs[i] - xs[j]) + sqrf(ys[i] - ys[j]));
            if (distance < 2*CIRCLE_RADIUS) {
               swap(&dxs[i], &dxs[j]);
               swap(&dys[i], &dys[j]);
            }
        }

        olivec_circle(oc, xs[i], ys[i], CIRCLE_RADIUS, colors[i]);
    }
    return oc;
}
