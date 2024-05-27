#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

struct EventHandler {
  SDL_Event event;
  int quit;
  // int pause;
  bool pause;
};

struct EventHandler *event_handler_create(void);
void event_handler_destroy(struct EventHandler *);

void handle_events(struct EventHandler *);

#endif
