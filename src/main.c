#include "../include/boid.h"
#include "../include/event_handler.h"
#include "../include/quad_tree.h"
#include "../include/ui.h"
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[]) {
  srand(time(NULL));
  struct UI *ui = ui_create("Boids", 1600, 1000);
  struct EventHandler *eventHandler = event_handler_create();

  int boidCount = 1000;
  struct Boid **boids = boids_create(ui, boidCount);

  // struct Rectangle rect = {ui->sizeX / 2, ui->sizeY / 2, ui->sizeX,
  // ui->sizeY};
  // struct QuadTree *quadTree = quad_tree_create(
  //     (struct Rectangle){ui->sizeX / 2, ui->sizeY / 2, ui->sizeX,
  //     ui->sizeY});

  const int FPS = 60;
  const int FRAME_DELAY = 1000 / FPS;
  Uint32 frameStart;
  int frameTime;

  while (!eventHandler->quit) {
    frameStart = SDL_GetTicks();

    handle_events(eventHandler);
    clear_screen(ui);

    struct QuadTree *quadTree = quad_tree_create(
        (struct Rectangle){ui->sizeX / 2, ui->sizeY / 2, ui->sizeX, ui->sizeY});

    for (int i = 0; i < boidCount; i++) {
      insert(quadTree, boids[i]);
    }

    for (int i = 0; i < boidCount; i++) {
      int mateCount;
      struct Boid **mates = query(quadTree, boids[i], &mateCount);
      find_flock(mates, mateCount, boids[i]);
      free(mates);

      // printf("queried: %d\n", mateCount);
      // if (i == 55) {
      //   boids[i]->length = 40;
      //   boids[i]->width = 30;
      //   draw_boid(ui, boids[i]);
      //
      //   for (int j = 0; j < mateCount; j++) {
      //     draw_boid_red(ui, mates[j]);
      //   }
      // }

      // find_flock(boids, boidCount, boids[i]);

      // printf(" brute force: %d\n", boids[i]->flockSize);

      separation(boids[i]);
      alignment(boids[i]);
      cohesion(boids[i]);
      fly(boids[i]);

      // wrap_around(ui, boids[i]);
      avoid_border(ui, boids[i]);
      draw_boid(ui, boids[i]);
    }
    // printf("Self: %f\n", boids[55]->vel[0]);
    boids[55]->width = 40;
    boids[55]->length = 60;
    draw_boid_red(ui, boids[55]);
    for (int i = 0; i < boids[55]->flockSize; i++) {
      draw_boid_red(ui, boids[55]->mates[i]);
    }

    quad_tree_draw(quadTree, ui);
    SDL_RenderPresent(ui->renderer);

    quad_tree_destroy(quadTree);

    frameTime = SDL_GetTicks() - frameStart;
    if (FRAME_DELAY > frameTime) {
      SDL_Delay(FRAME_DELAY - frameTime);
    }
  }

  // Clean up.
  ui_destroy(ui);
  event_handler_destroy(eventHandler);
  boids_destroy(boids, boidCount);
  // quad_tree_destroy(quadTree);

  SDL_Quit();

  return 0;
}
