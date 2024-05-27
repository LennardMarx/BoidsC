#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include <SDL2/SDL.h>
#include <stdlib.h>

struct AnimatedSprite {
  SDL_Surface *surface;
  SDL_Texture *texture;
  SDL_Rect position;
  SDL_Rect frame;
  SDL_Point point;
};

struct AnimatedSprite *animated_sprite_create(SDL_Renderer *, char *);
void animated_sprite_destroy(struct AnimatedSprite *);

void animated_sprite_draw(struct AnimatedSprite *, int, int, int, int);
void animated_sprite_play_frame(struct AnimatedSprite *, int, int, int, int,
                                int);
void animated_sprite_render(struct AnimatedSprite *, SDL_Renderer *, float *);

#endif
