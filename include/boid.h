#ifndef BOID_H
#define BOID_H

#include "../include/cglm/include/cglm/vec2.h"
#include "../include/ui.h"
#include <stdlib.h>

struct Boid {
  vec2 pos;
  vec2 vel;
  vec2 acc;
  float vision;

  int length;
  int width;

  vec2 flockCenter;
  vec2 flockVel;
  int flockSize;
};

struct Boid *boid_create(struct UI *);
struct Boid **boids_create(struct UI *, int);
void boid_destroy(struct Boid *);
void boids_destroy(struct Boid **, int);

void find_flock(struct Boid **, int, struct Boid *);
void align(struct Boid *);

void fly(struct Boid *);
void draw_boid(struct UI *, struct Boid *);
void wrap_around(struct UI *, struct Boid *);

#endif
