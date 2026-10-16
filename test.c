#include "olive.c/demos/vc.c"
#include <stdint.h>

#define WIDTH 960
#define HEIGHT 720
#define BACKGROUND_COLOR 0xFF181818
#define CIRCLE_RADIUS 100

static uint32_t pixels[WIDTH * HEIGHT];

static float xs[] = {100, 300, 500};
static float ys[] = {100, 300, 500};
static float dxs[] = {10, 8, 5};
static float dys[] = {7, 10, 6};
static uint32_t colors[] = {OLIVEC_RGBA(143, 99, 21, 255),
                            OLIVEC_RGBA(21, 25, 143, 255),
                            OLIVEC_RGBA(143, 21, 81, 255)};

float sqrtf(float x);
float atan2f(float y, float x);
float sinf(float x);
float cosf(float x);
float acosf(float x);

#define PI 3.14159265359

float sqrf(float x) { return x * x; }

float correct_d(float x, float dx, float dt, float border) {
  float perimeter = x + dx * dt;
  // if there not allready outside the bounds, then make sure they go back
  if (perimeter - CIRCLE_RADIUS < 0) {
    return OLIVEC_ABS(float, dx);
  }
  if (perimeter + CIRCLE_RADIUS >= border) {
    return -OLIVEC_ABS(float, dx);
  }
  return dx;
}

void wall_bounce(int i, float dt) {
  dxs[i] = correct_d(xs[i], dxs[i], dt, WIDTH);
  xs[i] = xs[i] + dxs[i];
  dys[i] = correct_d(ys[i], dys[i], dt, HEIGHT);
  ys[i] = ys[i] + dys[i];
}

Olivec_Canvas vc_render(float dt) {
  Olivec_Canvas oc = olivec_canvas(pixels, WIDTH, HEIGHT, WIDTH);

  olivec_fill(oc, BACKGROUND_COLOR);

  // Circle
  int n = *(&xs + 1) - xs;
  for (int i = 0; i < n; i++) {
    wall_bounce(i, dt);

    for (int j = 0; j < n; j++) {
      if (i == j)
        continue;
      // should include dx and dy, and should only flip the values if they are
      // not already going away from each other
      float distance = sqrtf(sqrf(xs[i] - xs[j]) + sqrf(ys[i] - ys[j]));
      if (distance < 2 * CIRCLE_RADIUS) {
        int approach = OLIVEC_SIGN(float, xs[i] - xs[j]) *
                       OLIVEC_SIGN(float, dxs[i] - dxs[j]);
        if (approach == -1)
          OLIVEC_SWAP(float, dxs[i], dxs[j]);
        approach = OLIVEC_SIGN(float, ys[i] - ys[j]) *
                   OLIVEC_SIGN(float, dys[i] - dys[j]);
        if (approach == -1)
          OLIVEC_SWAP(float, dys[i], dys[j]);
      }
    }

    olivec_circle(oc, xs[i], ys[i], CIRCLE_RADIUS, colors[i]);
  }
  return oc;
}
