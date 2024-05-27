#include "../include/quad_tree.h"

struct QuadTree *quad_tree_create(struct Rectangle rect) {
  struct QuadTree *qt = malloc(sizeof(struct QuadTree));
  qt->rect = rect;
  qt->children = NULL;
  qt->boids = NULL;
  qt->isDivided = false;
  qt->capacity = 3;
  qt->boidCount = 0;
  return qt;
}
void quad_tree_destroy(struct QuadTree *qt) {
  // if (qt->children != NULL) {
  if (qt->isDivided) {
    for (int i = 0; i < 4; i++) {
      quad_tree_destroy(qt->children[i]);
    }
    free(qt->children);
    qt->children = NULL;
  }
  if (qt->boids != NULL) {
    free(qt->boids);
    qt->boids = NULL;
  }
  if (qt != NULL) {
    free(qt);
  }
}

bool contains(struct QuadTree *qt, struct Boid *boid) {
  int left = qt->rect.x - qt->rect.w / 2;
  int right = qt->rect.x + qt->rect.w / 2;
  int up = qt->rect.y - qt->rect.h / 2;
  int down = qt->rect.y + qt->rect.h / 2;

  if (boid->pos[0] >= left && boid->pos[0] < right && boid->pos[1] >= up &&
      boid->pos[1] < down) {
    return true;
  } else {
    return false;
  }
}

struct Boid **query(struct QuadTree *qt, struct Boid *boid, int *count) {
  struct Boid **found = malloc(sizeof(struct Boid *));
  // int foundCount = 0;
  *count = 0;
  int total = 0;

  vec2 pos;
  glm_vec2(boid->pos, pos);
  float rad = boid->vision;
  int left = qt->rect.x - qt->rect.w / 2;
  int right = qt->rect.x + qt->rect.w / 2;
  int up = qt->rect.y - qt->rect.h / 2;
  int down = qt->rect.y + qt->rect.h / 2;
  if (pos[0] - rad > right || pos[0] + rad < left || pos[1] - rad > down ||
      pos[1] + rad < up) {
    free(found);
    return NULL;
  } else {
    for (int i = 0; i < qt->boidCount; i++) {
      if (contains(qt, qt->boids[i])) {
        // *count++;
        *count += 1;
        found = realloc(found, *count * sizeof(struct Boid *));
        found[*count - 1] = qt->boids[i];
      }
    }
    if (qt->isDivided) {
      int count0;
      struct Boid **found0 = query(qt->children[0], boid, &count0);
      // printf("Mates: %d\n", count0);
      int count1;
      struct Boid **found1 = query(qt->children[1], boid, &count1);
      int count2;
      struct Boid **found2 = query(qt->children[2], boid, &count2);
      int count3;
      struct Boid **found3 = query(qt->children[3], boid, &count3);
      total = *count + count0 + count1 + count2 + count3;
      // total = *count + count0 + count1 + count2;
      found = realloc(found, total * sizeof(struct Boid *));
      memcpy(found + *count, found0, count0 * sizeof(struct Boid *));
      memcpy(found + *count + count0, found1, count1 * sizeof(struct Boid *));
      memcpy(found + *count + count0 + count1, found2,
             count2 * sizeof(struct Boid *));
      memcpy(found + *count + count0 + count1 + count2, found3,
             count3 * sizeof(struct Boid *));
      *count = total;
      free(found0);
      free(found1);
      free(found2);
      free(found3);
    }
    return found;
    free(found);
  }
}

void insert(struct QuadTree *qt, struct Boid *boid) {
  if (!contains(qt, boid)) {
    return;
  }
  if (qt->boidCount < qt->capacity) {
    // printf("CONTAINS!\n");
    if (qt->boids == NULL) {
      qt->boidCount++;
      qt->boids = malloc(sizeof(struct Boid *));
      qt->boids[qt->boidCount - 1] = boid;
      // }
    } else {
      qt->boidCount++;
      qt->boids = realloc(qt->boids, sizeof(struct Boid *) * (qt->boidCount));
      qt->boids[qt->boidCount - 1] = boid;
    }
  } else {
    if (!qt->isDivided) {
      subdivide(qt);
    }
    insert(qt->children[0], boid);
    insert(qt->children[1], boid);
    insert(qt->children[2], boid);
    insert(qt->children[3], boid);
  }
}

void subdivide(struct QuadTree *qt) {
  if (qt->children != NULL) {
    free(qt->children);
  }
  qt->children = malloc(4 * sizeof(struct QuadTree *));
  qt->children[0] = quad_tree_create((struct Rectangle){
      qt->rect.x - qt->rect.w / 4, qt->rect.y - qt->rect.h / 4, qt->rect.w / 2,
      qt->rect.h / 2});
  qt->children[1] = quad_tree_create((struct Rectangle){
      qt->rect.x + qt->rect.w / 4, qt->rect.y - qt->rect.h / 4, qt->rect.w / 2,
      qt->rect.h / 2});
  qt->children[2] = quad_tree_create((struct Rectangle){
      qt->rect.x - qt->rect.w / 4, qt->rect.y + qt->rect.h / 4, qt->rect.w / 2,
      qt->rect.h / 2});
  qt->children[3] = quad_tree_create((struct Rectangle){
      qt->rect.x + qt->rect.w / 4, qt->rect.y + qt->rect.h / 4, qt->rect.w / 2,
      qt->rect.h / 2});
  qt->isDivided = true;
}

void quad_tree_draw(struct QuadTree *qt, struct UI *ui) {
  if (qt->children == NULL) {
    SDL_RenderDrawLine(ui->renderer, qt->rect.x - qt->rect.w / 2,
                       qt->rect.y - qt->rect.h / 2, qt->rect.x + qt->rect.w / 2,
                       qt->rect.y - qt->rect.h / 2);
    SDL_RenderDrawLine(ui->renderer, qt->rect.x - qt->rect.w / 2,
                       qt->rect.y - qt->rect.h / 2, qt->rect.x - qt->rect.w / 2,
                       qt->rect.y + qt->rect.h / 2);
    SDL_RenderDrawLine(ui->renderer, qt->rect.x + qt->rect.w / 2,
                       qt->rect.y - qt->rect.h / 2, qt->rect.x + qt->rect.w / 2,
                       qt->rect.y + qt->rect.h / 2);
    SDL_RenderDrawLine(ui->renderer, qt->rect.x - qt->rect.w / 2,
                       qt->rect.y + qt->rect.h / 2, qt->rect.x + qt->rect.w / 2,
                       qt->rect.y + qt->rect.h / 2);
  } else if (qt->children != NULL) {
    for (int i = 0; i < 4; i++) {
      quad_tree_draw(qt->children[i], ui);
    }
  }
}
