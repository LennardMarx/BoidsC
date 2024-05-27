#include "../include/event_handler.h"

struct EventHandler *event_handler_create(void) {
  struct EventHandler *eh = malloc(sizeof(struct EventHandler));
  eh->quit = 0;
  eh->pause = 0;
  eh->click = 0;
  return eh;
}

void handle_events(struct EventHandler *eh) {
  while (SDL_PollEvent(&eh->event) != 0) {
    // User requests quit
    if (eh->event.type == SDL_QUIT) {
      eh->quit = 1;
    }
    switch (eh->event.type) {
    case SDL_MOUSEBUTTONDOWN:
      eh->click = 1;
      // printf("CLICK\n");
      eh->pause = !eh->pause;
      eh->click = 0;
      break;
    case SDL_MOUSEBUTTONUP:
      // if (eh->event.button.clicks == SDL_BUTTON_LEFT)
      eh->click = 0;
      break;
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
