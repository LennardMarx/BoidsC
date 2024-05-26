#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include "../include/boid.h"
#include "../include/ui.h"
#include <SDL2/SDL.h>
#include <stdlib.h>

struct Rectangle {
  int x, y, w, h;
};

struct QuadTree {
  struct QuadTree **children;
  struct Boid **boids;
  int boidCount;

  struct Rectangle rect;
  bool isDivided;

  int capacity;
};

struct QuadTree *quad_tree_create(struct Rectangle);
void quad_tree_destroy(struct QuadTree *);

void subdivide(struct QuadTree *);
void insert(struct QuadTree *, struct Boid *);

bool contains(struct QuadTree *, struct Boid *);
struct Boid **query(struct QuadTree *, struct Boid *);

void quad_tree_clear(struct QuadTree *);
void quad_tree_draw(struct QuadTree *, struct UI *);

#endif
