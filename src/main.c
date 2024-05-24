#include "../include/boid.h"
#include "../include/event_handler.h"
#include "../include/ui.h"
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[]) {
  srand(time(NULL));
  struct UI *ui = ui_create("Boids", 1600, 1000);
  struct EventHandler *eventHandler = event_handler_create();

  int boidCount = 1000;
  struct Boid **boids = boids_create(ui, boidCount);

  const int FPS = 60;
  const int FRAME_DELAY = 1000 / FPS;
  Uint32 frameStart;
  int frameTime;

  while (!eventHandler->quit) {
    frameStart = SDL_GetTicks();

    handle_events(eventHandler);
    clear_screen(ui);

    for (int i = 0; i < boidCount; i++) {
      find_flock(boids, boidCount, boids[i]);

      separation(boids[i]);
      alignment(boids[i]);
      cohesion(boids[i]);
      fly(boids[i]);

      // wrap_around(ui, boids[i]);
      avoid_border(ui, boids[i]);
      draw_boid(ui, boids[i]);
    }
    // printf("Self: %f\n", boids[55]->vel[0]);

    SDL_RenderPresent(ui->renderer);

    frameTime = SDL_GetTicks() - frameStart;
    if (FRAME_DELAY > frameTime) {
      SDL_Delay(FRAME_DELAY - frameTime);
    }
  }

  // Clean up.
  ui_destroy(ui);
  event_handler_destroy(eventHandler);
  boids_destroy(boids, boidCount);

  SDL_Quit();

  return 0;
}
