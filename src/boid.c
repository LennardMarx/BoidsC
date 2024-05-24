#include "../include/boid.h"

void find_flock(struct Boid **boids, int count, struct Boid *boid) {
  free(boid->mates);
  boid->mates = calloc(0, sizeof(struct Boid *));
  boid->flockSize = 0;
  glm_vec2_zero(boid->flockCenter);
  glm_vec2_zero(boid->flockVel);

  // Get flock mates and info.
  for (int i = 0; i < count; i++) {
    if (boids[i] == boid) {
      continue;
    }
    if (glm_vec2_distance(boids[i]->pos, boid->pos) < boid->vision) {
      boid->flockSize++;
      boid->mates =
          realloc(boid->mates, sizeof(struct Boid *) * boid->flockSize);
      boid->mates[boid->flockSize - 1] = boids[i];
      glm_vec2_add(boid->flockCenter, boids[i]->pos, boid->flockCenter);
      glm_vec2_add(boid->flockVel, boids[i]->vel, boid->flockVel);
    }
  }

  // Average collected info over local flock size.
  if (boid->flockSize != 0) {
    boid->flockCenter[0] /= boid->flockSize;
    boid->flockCenter[1] /= boid->flockSize;
    boid->flockVel[0] /= boid->flockSize;
    boid->flockVel[1] /= boid->flockSize;
  } else {
    glm_vec2(boid->pos, boid->flockCenter);
    glm_vec2(boid->vel, boid->flockVel);
  }
}

void limit_force(struct Boid *boid, vec2 force) {
  if (glm_vec2_norm(force) > boid->maxForce) {
    glm_vec2_mul(force, (vec2){boid->maxForce, boid->maxForce}, force);
  }
}

// printf("WD: %f\n", weightedDistance[0]);
void separation(struct Boid *boid) {
  vec2 weightedDistance;
  glm_vec2_zero(weightedDistance);
  if (boid->flockSize != 0) {
    for (int i = 0; i < boid->flockSize; i++) {
      vec2 dist;
      glm_vec2_sub(boid->pos, boid->mates[i]->pos, dist);
      if (glm_vec2_norm(dist) != 0) {
        weightedDistance[0] += dist[0] / glm_vec2_norm(dist);
        weightedDistance[1] += dist[1] / glm_vec2_norm(dist);
      }
    }
    weightedDistance[0] /= boid->flockSize;
    weightedDistance[1] /= boid->flockSize;
  }
  glm_vec2_normalize(weightedDistance);
  glm_vec2_mul(weightedDistance, (vec2){boid->maxVel, boid->maxVel},
               weightedDistance);
  vec2 force;
  glm_vec2_sub(weightedDistance, boid->vel, force);
  limit_force(boid, force);
  glm_vec2_mul(force, (vec2){1.3f, 1.3f}, force);
  glm_vec2_add(boid->acc, force, boid->acc);
}

void alignment(struct Boid *boid) {
  glm_vec2_normalize(boid->flockVel);
  glm_vec2_mul(boid->flockVel, (vec2){boid->maxVel, boid->maxVel},
               boid->flockVel);

  vec2 force;
  glm_vec2_sub(boid->flockVel, boid->vel, force);
  limit_force(boid, force);
  glm_vec2_mul(force, (vec2){0.5f, 0.5f}, force);
  glm_vec2_add(boid->acc, force, boid->acc);
}

void cohesion(struct Boid *boid) {
  vec2 desiredVel;
  if (boid->flockCenter != boid->pos) {
    glm_vec2_sub(boid->flockCenter, boid->pos, desiredVel);
  } else {
    glm_vec2(boid->vel, desiredVel);
  }

  glm_vec2_normalize(desiredVel);
  glm_vec2_mul(desiredVel, (vec2){boid->maxVel, boid->maxVel}, desiredVel);

  vec2 force;
  glm_vec2_sub(desiredVel, boid->vel, force);
  limit_force(boid, force);
  glm_vec2_mul(force, (vec2){1.2f, 1.2f}, force);
  glm_vec2_add(boid->acc, force, boid->acc);
}
void avoid_border(struct UI *ui, struct Boid *boid) {
  if (boid->pos[0] < 50.0f) {
    vec2 desiredVel;
    glm_vec2((vec2){1.0f, 0.0f}, desiredVel);
    glm_vec2_mul(desiredVel, (vec2){boid->maxVel, boid->maxVel}, desiredVel);

    vec2 force;
    glm_vec2_sub(desiredVel, boid->vel, force);
    limit_force(boid, force);
    glm_vec2_add(boid->acc, force, boid->acc);
  } else if (boid->pos[0] > ui->sizeX - 50.0f) {
    vec2 desiredVel;
    glm_vec2((vec2){-1.0f, 0.0f}, desiredVel);
    glm_vec2_mul(desiredVel, (vec2){boid->maxVel, boid->maxVel}, desiredVel);

    vec2 force;
    glm_vec2_sub(desiredVel, boid->vel, force);
    limit_force(boid, force);
    glm_vec2_add(boid->acc, force, boid->acc);
  } else if (boid->pos[1] < 50.0f) {
    vec2 desiredVel;
    glm_vec2((vec2){0.0f, 1.0f}, desiredVel);
    glm_vec2_mul(desiredVel, (vec2){boid->maxVel, boid->maxVel}, desiredVel);

    vec2 force;
    glm_vec2_sub(desiredVel, boid->vel, force);
    limit_force(boid, force);
    glm_vec2_add(boid->acc, force, boid->acc);
  } else if (boid->pos[1] > ui->sizeY - 50.0f) {
    vec2 desiredVel;
    glm_vec2((vec2){0.0f, -1.0f}, desiredVel);
    glm_vec2_mul(desiredVel, (vec2){boid->maxVel, boid->maxVel}, desiredVel);

    vec2 force;
    glm_vec2_sub(desiredVel, boid->vel, force);
    limit_force(boid, force);
    glm_vec2_add(boid->acc, force, boid->acc);
  }
}

void wrap_around(struct UI *ui, struct Boid *boid) {
  if (boid->pos[0] <= 0.0f) {
    boid->pos[0] = ui->sizeX;
  } else if (boid->pos[0] >= ui->sizeX) {
    boid->pos[0] = 0.0f;
  } else if (boid->pos[1] <= 0.0f) {
    boid->pos[1] = ui->sizeY;
  } else if (boid->pos[1] >= ui->sizeY) {
    boid->pos[1] = 0.0f;
  }
}

void draw_boid(struct UI *ui, struct Boid *boid) {
  vec2 dir;
  glm_vec2(boid->vel, dir);
  glm_vec2_normalize(dir);

  SDL_RenderDrawLine(ui->renderer, boid->pos[0], boid->pos[1],
                     boid->pos[0] + dir[0] * boid->length,
                     boid->pos[1] + dir[1] * boid->length);
  SDL_RenderDrawLine(ui->renderer, boid->pos[0] + dir[1] * boid->width / 2,
                     boid->pos[1] - dir[0] * boid->width / 2,
                     boid->pos[0] - dir[1] * boid->width / 2,
                     boid->pos[1] + dir[0] * boid->width / 2);
  SDL_RenderDrawLine(ui->renderer, boid->pos[0] + dir[1] * boid->width / 2,
                     boid->pos[1] - dir[0] * boid->width / 2,
                     boid->pos[0] + dir[0] * boid->length,
                     boid->pos[1] + dir[1] * boid->length);
  SDL_RenderDrawLine(ui->renderer, boid->pos[0] - dir[1] * boid->width / 2,
                     boid->pos[1] + dir[0] * boid->width / 2,
                     boid->pos[0] + dir[0] * boid->length,
                     boid->pos[1] + dir[1] * boid->length);
}

struct Boid **boids_create(struct UI *ui, int count) {
  struct Boid **boids = (struct Boid **)malloc(count * sizeof(struct Boid *));
  for (int i = 0; i < count; i++) {
    boids[i] = boid_create(ui);
  }
  return boids;
}

void fly(struct Boid *boid) {
  if (glm_vec2_norm(boid->vel) < boid->maxVel) {
    glm_vec2_normalize(boid->vel);
    glm_vec2_mul(boid->vel, (vec2){boid->maxVel, boid->maxVel}, boid->vel);
  }

  glm_vec2_add(boid->pos, boid->vel, boid->pos);
  glm_vec2_add(boid->vel, boid->acc, boid->vel);
  glm_vec2_zero(boid->acc);
}

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
  boid->width = 10.0f;

  boid->mates = calloc(0, sizeof(struct Boid *));

  return boid;
}
void boid_destroy(struct Boid *boid) { free(boid); }
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
