#include "../include/boid.h"

struct Boid *boid_create(struct UI *ui) {
  struct Boid *boid = malloc(sizeof(struct Boid));
  if (boid == NULL) {
    return NULL;
  }
  boid->maxForce = 0.05f;
  boid->maxVel = 4.0f;

  boid->pos[0] = rand() % (ui->sizeX + 1);
  boid->pos[1] = rand() % (ui->sizeY + 1);

  boid->vel[0] = rand() - RAND_MAX / 2;
  boid->vel[1] = rand() - RAND_MAX / 2;
  glm_vec2_normalize(boid->vel);
  glm_vec2_mul(boid->vel, (vec2){boid->maxVel, boid->maxVel}, boid->vel);

  glm_vec2((vec2){0, 0}, boid->acc);

  glm_vec2((vec2){0, 0}, boid->flockCenter);
  glm_vec2((vec2){0, 0}, boid->flockVel);
  boid->flockSize = 0;

  boid->vision = 50.0f;

  boid->length = 20.0f;
  boid->width = 40.0f;

  boid->mates = calloc(0, sizeof(struct Boid *));

  boid->animationFrame = rand() % (12 + 1);

  return boid;
}

struct Boid **boids_create(struct UI *ui, int count) {
  struct Boid **boids = (struct Boid **)malloc(count * sizeof(struct Boid *));
  for (int i = 0; i < count; i++) {
    boids[i] = boid_create(ui);
  }
  return boids;
}

// void boid_destroy(struct Boid *boid) { free(boid); }

void boids_destroy(struct Boid **boids, int count) {
  for (int i = 0; i < count; i++) {
    // for (int j = 0; j < boids[i]->flockSize; j++) {
    //   free(boids[i]->mates[j]);
    // }
    free(boids[i]->mates);
    free(boids[i]);
  }
  free(boids);
}
