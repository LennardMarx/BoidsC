#include "../include/boid.h"

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
