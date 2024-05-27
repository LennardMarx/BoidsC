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

  int boidCount = 2000;
  struct Boid **boids = boids_create(ui, boidCount);

  const int FPS = 60;
  const int FRAME_DELAY = 1000 / FPS;
  Uint32 frameStart;
  int frameTime;

  int mouseX, mouseY;

  while (!eventHandler->quit) {
    frameStart = SDL_GetTicks();

    SDL_GetMouseState(&mouseX, &mouseY);
    vec2 mouse;
    glm_vec2((vec2){mouseX, mouseY}, mouse);

    // printf("Mouse: %f, %f\n", mouse[0], mouse[1]);

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

      // find_flock(boids, boidCount, boids[i]);

      separation(boids[i]);
      alignment(boids[i]);
      cohesion(boids[i]);

      // wrap_around(ui, boids[i]);
      avoid_border(ui, boids[i]);
      avoid_mouse(&mouse, boids[i]);

      fly(boids[i]);
      draw_boid(ui, boids[i]);
    }

    // quad_tree_draw(quadTree, ui);

    SDL_RenderPresent(ui->renderer);

    quad_tree_destroy(quadTree);

    frameTime = SDL_GetTicks() - frameStart;
    if (FRAME_DELAY > frameTime) {
      SDL_Delay(FRAME_DELAY - frameTime);
    }
    // printf("Frame Time: %d\n", frameTime);
  }

  // Clean up.
  ui_destroy(ui);
  event_handler_destroy(eventHandler);
  boids_destroy(boids, boidCount);

  SDL_Quit();

  return 0;
}
