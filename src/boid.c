#include "../include/boid.h"

void find_flock(struct Boid **boids, int count, struct Boid *boid) {
  // Reset values.
  // flock->mates.clear();
  boid->flockSize = 0;
  glm_vec2_zero(boid->flockCenter);
  glm_vec2_zero(boid->flockVel);
  // flock->center = glm::vec2(0, 0);
  // flock->vel = glm::vec2(0, 0);

  // Get flock mates and info.
  for (int i = 0; i < count; i++) {
    if (boids[i] == boid) {
      continue;
    }
    // if (glm::length(boids[j]->pos - pos) < vision) {
    //   flock->mates.push_back(boids[j]);
    //   flock->center += boids[j]->pos;
    //   flock->vel += boids[j]->vel;
    // }
    if (glm_vec2_distance(boids[i]->pos, boid->pos) < boid->vision) {
      boid->flockSize++;
      glm_vec2_add(boid->flockCenter, boids[i]->pos, boid->flockCenter);
      glm_vec2_add(boid->flockVel, boids[i]->vel, boid->flockVel);
    }
  }

  // Average collected info over local flock size.
  // if (flock->mates.size() != 0) {
  //   flock->center /= flock->mates.size();
  //   flock->vel /= flock->mates.size();
  // } else {
  //   flock->center = this->pos;
  //   flock->vel = this->vel;
  // }
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
void align(struct Boid *boid) {
  // flock->vel = glm::normalize(flock->vel) * maxVel;
  // glm::vec2 force = flock->vel - vel;
  //
  // return limitForce(force);
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
  glm_vec2_add(boid->pos, boid->vel, boid->pos);
  glm_vec2_add(boid->vel, boid->acc, boid->vel);
}

struct Boid *boid_create(struct UI *ui) {
  struct Boid *boid = malloc(sizeof(struct Boid));
  if (boid == NULL) {
    return NULL;
  }
  boid->pos[0] = rand() % (ui->sizeX + 1);
  boid->pos[1] = rand() % (ui->sizeY + 1);

  boid->vel[0] = rand() - RAND_MAX / 2;
  boid->vel[1] = rand() - RAND_MAX / 2;
  glm_vec2_normalize(boid->vel);

  boid->acc[0] = 0.0f;
  boid->acc[1] = 0.0f;

  boid->vision = 50.0f;
  boid->length = 30.0f;
  boid->width = 20.0f;

  boid->flockCenter[0] = 0;
  boid->flockCenter[1] = 0;
  boid->flockVel[0] = 0;
  boid->flockVel[1] = 0;
  boid->flockSize = 0;

  return boid;
}
void boid_destroy(struct Boid *boid) { free(boid); }
void boids_destroy(struct Boid **boids, int count) {
  for (int i = 0; i < count; i++) {
    free(boids[i]);
  }
  free(boids);
}
