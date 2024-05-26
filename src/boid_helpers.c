#include "../include/boid.h"

void limit_force(struct Boid *boid, vec2 force) {
  if (glm_vec2_norm(force) > boid->maxForce) {
    glm_vec2_mul(force, (vec2){boid->maxForce, boid->maxForce}, force);
  }
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
