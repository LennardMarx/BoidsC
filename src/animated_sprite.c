#include "../include/animated_sprite.h"

struct AnimatedSprite *animated_sprite_create(SDL_Renderer *renderer,
                                              char *filepath) {
  struct AnimatedSprite *anim = malloc(sizeof(struct AnimatedSprite));
  anim->surface = SDL_LoadBMP(filepath);
  anim->texture = SDL_CreateTextureFromSurface(renderer, anim->surface);
  return anim;
}
void animated_sprite_destroy(struct AnimatedSprite *anim) {
  SDL_FreeSurface(anim->surface);
  SDL_DestroyTexture(anim->texture);
  free(anim);
}

void animated_sprite_draw(struct AnimatedSprite *anim, int x, int y, int w,
                          int h) {
  anim->position.x = x;
  anim->position.y = y;
  anim->position.w = w;
  anim->position.h = h;

  anim->point.x = w / 2; // center point of rotation
  anim->point.y = h / 2;
}
void animated_sprite_play_frame(struct AnimatedSprite *anim, int x, int y,
                                int w, int h, int frameNumber) {
  anim->frame.x = x;
  anim->frame.y = y + h * frameNumber;
  anim->frame.w = w;
  anim->frame.h = h;
}
void animated_sprite_render(struct AnimatedSprite *anim, SDL_Renderer *renderer,
                            float *angle) {
  SDL_RenderCopyEx(renderer, anim->texture, &anim->frame, &anim->position,
                   *angle, &anim->point, SDL_FLIP_NONE);
}
