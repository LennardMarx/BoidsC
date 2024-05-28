#include "../include/event_handler.h"

struct EventHandler *event_handler_create(void) {
  struct EventHandler *eh = malloc(sizeof(struct EventHandler));
  eh->quit = 0;
  eh->pause = 0;
  eh->click = 0;
  eh->showQuadTree = 0;
  return eh;
}

void handle_events(struct EventHandler *eh, struct UI *ui, vec2 *mouse) {
  while (SDL_PollEvent(&eh->event) != 0) {
    // User requests quit
    if (eh->event.type == SDL_QUIT) {
      eh->quit = 1;
    }
    switch (eh->event.type) {
    case SDL_MOUSEBUTTONDOWN:
      if ((*mouse)[0] > ui->sizeX - 400 && (*mouse)[1] < 300)
        eh->showQuadTree = !eh->showQuadTree;
      break;
    // case SDL_MOUSEBUTTONUP:
    //   break;
    case SDL_KEYDOWN:
      switch (eh->event.key.keysym.sym) {
      case SDLK_ESCAPE:
        eh->quit = 1;
        break;
      case SDLK_q:
        eh->quit = 1;
        break;
      case SDLK_SPACE:
        eh->pause = !eh->pause;
        break;
      default:
        break;
      }
    }
  }
}

void event_handler_destroy(struct EventHandler *eh) { free(eh); }
