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

// struct Boid** qu

// void quad_tree_clear(struct QuadTree *qt) {
//   // if (qt->children != NULL) {
//   if (qt == NULL) {
//     return;
//   }
//   if (qt->isDivided) {
//     // printf("IS DIVIDED\n");
//     for (int i = 0; i < 4; i++) {
//       quad_tree_clear(qt->children[i]);
//     }
//     free(qt->children);
//     qt->children = NULL;
//   }
//   if (qt->boids != NULL) {
//     free(qt->boids);
//     qt->boids = NULL;
//   }
//   qt->isDivided = false;
//   qt->boidCount = 0;
// }

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
  // if (qt->children == NULL) {
  //   qt->children = malloc(4 * sizeof(struct QuadTree *));
  // }
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
  // SDL_SetRenderDrawColor(ui->getRenderer(), 249, 245, 215, 50); //
  // grub-light
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
  if (qt->children != NULL) {
    for (int i = 0; i < 4; i++) {
      quad_tree_draw(qt->children[i], ui);
    }
  }
}
