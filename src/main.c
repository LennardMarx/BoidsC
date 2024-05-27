#include "../include/animated_sprite.h"
#include "../include/boid.h"
#include "../include/event_handler.h"
#include "../include/quad_tree.h"
#include "../include/textured_rectangle.h"
#include "../include/ui.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define PI 3.141592654

int main(int argc, char *argv[]) {
  if (chdir(SDL_GetBasePath()) != 0) {
    perror("chdir failed");
  }

  srand(time(NULL));

  struct UI *ui = ui_create("Boids", 1600, 1000);
  struct EventHandler *eventHandler = event_handler_create();

  struct TexturedRectangle *texturedRectangle =
      textured_rectangle_create(ui->renderer, "../resources/duck.bmp");
  struct AnimatedSprite *animatedSprite = animated_sprite_create(
      ui->renderer, "../resources/ducks_12_flap_outline.bmp");

  int boidCount = 2000;
  struct Boid **boids = boids_create(ui, boidCount);

  const int FPS = 60;
  const int FRAME_DELAY = 1000 / FPS;
  Uint32 frameStart;
  int frameTime;

  vec2 mouse;
  int mouseX, mouseY;

  int frameCount = 0;

  while (!eventHandler->quit) {
    frameStart = SDL_GetTicks();

    SDL_GetMouseState(&mouseX, &mouseY);
    glm_vec2((vec2){mouseX, mouseY}, mouse);

    handle_events(eventHandler);
    clear_screen(ui);

    struct QuadTree *quadTree = quad_tree_create(
        (struct Rectangle){(float)ui->sizeX / 2, (float)ui->sizeY / 2,
                           (float)ui->sizeX, (float)ui->sizeY});

    for (int i = 0; i < boidCount; i++) {
      insert(quadTree, boids[i]);
    }

    for (int i = 0; i < boidCount; i++) {
      int mateCount;
      struct Boid **mates = query(quadTree, boids[i], &mateCount);
      find_flock(mates, mateCount, boids[i]);
      free(mates);

      separation(boids[i]);
      alignment(boids[i]);
      cohesion(boids[i]);

      avoid_border(ui, boids[i]);
      avoid_mouse(&mouse, boids[i]);

      fly(boids[i]);
      boid_continue_animation(boids[i], &frameCount);
      // draw_boid(ui, boids[i]);
      // textured_rectangle_draw(texturedRectangle, boids[i]->pos[0] - 20,
      //                         boids[i]->pos[1] - 10, 40, 20);
      animated_sprite_draw(animatedSprite, boids[i]->pos[0] - 20,
                           boids[i]->pos[1] - 10, 40, 20);
      animated_sprite_play_frame(animatedSprite, 0, 0, 135, 65,
                                 boids[i]->animationFrame);
      float angle = -atan2(boids[i]->vel[0], boids[i]->vel[1]) * 180 / PI + 180;
      // float angle = 20.0f;
      // textured_rectangle_render(texturedRectangle, ui->renderer, &angle);
      animated_sprite_render(animatedSprite, ui->renderer, &angle);
    }

    if (eventHandler->pause) {
      quad_tree_draw(quadTree, ui);
    }

    SDL_RenderPresent(ui->renderer);

    quad_tree_destroy(quadTree);

    frameTime = SDL_GetTicks() - frameStart;
    if (FRAME_DELAY > frameTime) {
      SDL_Delay(FRAME_DELAY - frameTime);
    }
    frameCount++;
    // if (frameCount % 4 == 0) {
    //   animationFrame++;
    // }
    // if (animationFrame == 12) {
    //   animationFrame = 0;
    // }
    // printf("Frame Time: %d\n", frameTime);
  }

  // Clean up.
  ui_destroy(ui);
  event_handler_destroy(eventHandler);
  boids_destroy(boids, boidCount);
  textured_rectangle_destroy(texturedRectangle);
  animated_sprite_destroy(animatedSprite);

  SDL_Quit();

  return 0;
}
