#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
// #include <math.h>
#include <stdlib.h>

struct UI {
  SDL_Window *window;
  SDL_Renderer *renderer;
  int sizeX;
  int sizeY;
};

struct UI *ui_create(char *, int, int);
void ui_destroy(struct UI *);

void clear_screen(struct UI *);

#endif
