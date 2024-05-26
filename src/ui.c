#include "../include/ui.h"

void clear_screen(struct UI *ui) {
  SDL_SetRenderDrawColor(ui->renderer, 69, 133, 136, 255); // gruv-blue
  SDL_RenderClear(ui->renderer);
  SDL_SetRenderDrawColor(ui->renderer, 249, 245, 215, 150); // gruv-light
}

struct UI *ui_create(char *windowName, int width, int height) {
  struct UI *ui = malloc(sizeof(struct UI));
  ui->window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, width, height,
                                SDL_WINDOW_SHOWN);
  ui->renderer = SDL_CreateRenderer(ui->window, -1, SDL_RENDERER_ACCELERATED);
  ui->sizeX = width;
  ui->sizeY = height;
  return ui;
}

void ui_destroy(struct UI *ui) {
  SDL_DestroyWindow(ui->window);
  SDL_DestroyRenderer(ui->renderer);
  free(ui);
}
