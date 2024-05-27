#ifndef TEXTURED_RECTANGLE_H
#define TEXTURED_RECTANGLE_H

#include "../include/rectangle.h"
#include "../include/ui.h"
#include <SDL2/SDL.h>

struct TexturedRectangle {
  SDL_Surface *surface;
  SDL_Texture *texture;
  SDL_Rect rect;
  SDL_Point point;
};

struct TexturedRectangle *textured_rectangle_create(SDL_Renderer *, char *);
void textured_rectangle_destroy(struct TexturedRectangle *);

void textured_rectangle_draw(struct TexturedRectangle *, int, int, int, int);
void textured_rectangle_render(struct TexturedRectangle *, SDL_Renderer *,
                               float *);

#endif
