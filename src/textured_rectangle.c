#include "../include/textured_rectangle.h"

struct TexturedRectangle *textured_rectangle_create(SDL_Renderer *renderer,
                                                    char *filepath) {
  struct TexturedRectangle *tex = malloc(sizeof(struct TexturedRectangle));
  tex->surface = SDL_LoadBMP(filepath);
  tex->texture = SDL_CreateTextureFromSurface(renderer, tex->surface);
  return tex;
}

void textured_rectangle_destroy(struct TexturedRectangle *tex) {
  SDL_FreeSurface(tex->surface);
  SDL_DestroyTexture(tex->texture);
  free(tex);
}

void textured_rectangle_draw(struct TexturedRectangle *tex, int x, int y, int w,
                             int h) {
  // Create a rectangle
  tex->rect.x = x;
  tex->rect.y = y;
  tex->rect.w = w;
  tex->rect.h = h;

  tex->point.x = w / 2; // center point of rotation
  tex->point.y = h / 2;
}

void textured_rectangle_render(struct TexturedRectangle *tex,
                               SDL_Renderer *renderer, float *angle) {
  // SDL_RenderCopy(renderer, m_texture, NULL, &m_rectangle);
  SDL_RenderCopyEx(renderer, tex->texture, NULL, &tex->rect, *angle,
                   &tex->point, SDL_FLIP_NONE);
}
