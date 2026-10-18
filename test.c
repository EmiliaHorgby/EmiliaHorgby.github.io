#include <stdint.h>

#include "olive.c/demos/vc.c"

#define WIDTH 960
#define HEIGHT 720
#define BACKGROUND_COLOR 0xFF181818
#define CIRCLE_RADIUS 100
#define N_BALLS 5

static uint32_t pixels[WIDTH * HEIGHT];

static float xs[N_BALLS] = {800, 300, 300, 40, 700};
static float ys[N_BALLS] = {200, 100, 400, 70, 90};
static float dxs[N_BALLS] = {10, 5, 1, 15, 9};
static float dys[N_BALLS] = {2, 10, 6, 3, 1};
static float sizes[N_BALLS] = {100, 20, 50, 20, 15};

float sqrtf(float x);
float atan2f(float y, float x);
float sinf(float x);
float cosf(float x);
float acosf(float x);

#define PI 3.14159265359

float sqrf(float x) { return x * x; }


float correct_d(float x, float dx, float dt, float r, float border) {
   float perimeter = x + dx * dt;
   // if there not allready outside the bounds, then make sure they go back
   if (perimeter - r < 0) {
      return OLIVEC_ABS(float, dx);
   }
   if (perimeter + r >= border) {
      return -OLIVEC_ABS(float, dx);
   }
   return dx;
}

void wall_bounce(int i, float dt) {
   dxs[i] = correct_d(xs[i], dxs[i], dt, sizes[i], WIDTH);
   xs[i] = xs[i] + dxs[i];
   dys[i] = correct_d(ys[i], dys[i], dt, sizes[i], HEIGHT);
   ys[i] = ys[i] + dys[i];
}

float euclidean(float x, float y) { return sqrtf(sqrf(x) + sqrf(y)); }
void ball_bounce(int i, int j) {
   // should include dx and dy, and should only flip the values if they
   // are not already going away from each other
   float distance = euclidean(xs[i] - xs[j], ys[i] - ys[j]);
   if (distance < sizes[i] + sizes[j]) {
      float mass_i = sqrf(sizes[i]);
      float mass_j = sqrf(sizes[j]);
      float prop_mass_i = mass_i / (mass_i + mass_j);
      float prop_mass_j = mass_j / (mass_i + mass_j);

      float ldx = xs[i] - xs[j];
      float rdx = dxs[i] - dxs[j];
      float ldy = ys[i] - ys[j];
      float rdy = dys[i] - dys[j];
      float common_factor = (ldx * rdx + ldy * rdy) / sqrf(distance);

      // Avoids the balls getting stuck in each other
      if (OLIVEC_ABS(float, rdx) < 0.1) {
         dxs[i] = -OLIVEC_SIGN(float, ldx) * 0.1;
         dxs[j] = OLIVEC_SIGN(float, ldx) * 0.1;
      }

      int approach = OLIVEC_SIGN(float, ldx) * OLIVEC_SIGN(float, rdx);
      if (approach == -1) {
         dxs[i] -= 2 * prop_mass_j * ldx * common_factor;
         dxs[j] += 2 * prop_mass_i * ldx * common_factor;
      }

      // Avoids the balls getting stuck in each other
      if (OLIVEC_ABS(float, rdy) < 0.3) {
         dys[i] = OLIVEC_SIGN(float, ldy) * 0.3;
         dys[j] = -OLIVEC_SIGN(float, ldy) * 0.3;
      }

      approach = OLIVEC_SIGN(float, ldy) * OLIVEC_SIGN(float, rdy);
      if (approach == -1) {
         dys[i] -= 2 * prop_mass_j * ldy * common_factor;
         dys[j] += 2 * prop_mass_i * ldy * common_factor;
      }
   }
}

uint32_t rainbow(float x) {
   int r = 255 * (sinf(360 * x / (2 * N_BALLS)) + 1)/2;
   int g = 255 * (sinf(360 * x / (2 * N_BALLS) + 2 * PI / 3) +1) /2;
   int b = 255 * (sinf(360 * x / (2 * N_BALLS) + 4 * PI / 3) + 1) /2;
   return OLIVEC_RGBA(r, g, b, 255);
}

static float momentum = -1;
Olivec_Canvas vc_render(float dt) {

   float current_momentum = 0;
   float vals[] = {0, 0.2, 0.4, 0.8, 1};
   Olivec_Canvas oc = olivec_canvas(pixels, WIDTH, HEIGHT, WIDTH);
   olivec_fill(oc, BACKGROUND_COLOR);

   for (int i = 0; i < N_BALLS; i++) {
      wall_bounce(i, dt);
      for (int j = 0; j < i; j++) {
         ball_bounce(i, j);
      }
      olivec_circle(oc, xs[i], ys[i], sizes[i], rainbow(vals[i]));
   }
   return oc;
}
