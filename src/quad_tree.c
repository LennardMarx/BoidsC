#include "../include/quad_tree.h"

struct QuadTree *quad_tree_create(struct Rectangle rect) {
  struct QuadTree *qt = malloc(sizeof(struct QuadTree));
  qt->rect = rect;
  qt->children = NULL;
  qt->boids = NULL;
  qt->isDivided = false;
  qt->capacity = 3;
  return qt;
}
void quad_tree_destroy(struct QuadTree *qt) {
  if (qt->children != NULL) {
    for (int i = 0; i < 4; i++) {
      quad_tree_destroy(qt->children[i]);
    }
    free(qt->children);
  }
  if (qt->boids != NULL) {

    free(qt->boids);
  }
  free(qt);
}

void subdivide(struct QuadTree *qt) {
  qt->children = malloc(sizeof(struct QuadTree *) * 4);
  qt->children[0] = quad_tree_create((struct Rectangle){
      qt->rect.x - qt->rect.w / 4, qt->rect.y - qt->rect.h / 4, qt->rect.w / 2,
      qt->rect.h / 2});
  qt->children[1] = quad_tree_create((struct Rectangle){
      qt->rect.x + qt->rect.w / 4, qt->rect.y - qt->rect.h / 4, qt->rect.w / 2,
      qt->rect.h / 2});
  qt->children[2] = quad_tree_create((struct Rectangle){
      qt->rect.x - qt->rect.w / 4, qt->rect.y + qt->rect.h / 4, qt->rect.w / 2,
      qt->rect.h / 2});
  qt->children[3] = quad_tree_create((struct Rectangle){
      qt->rect.x + qt->rect.w / 4, qt->rect.y + qt->rect.h / 4, qt->rect.w / 2,
      qt->rect.h / 2});
  qt->isDivided = true;
}

void quad_tree_draw(struct QuadTree *qt, struct UI *ui) {
  // SDL_SetRenderDrawColor(ui->getRenderer(), 249, 245, 215, 50); // grub-light
  SDL_RenderDrawLine(ui->renderer, qt->rect.x - qt->rect.w / 2,
                     qt->rect.y - qt->rect.h / 2, qt->rect.x + qt->rect.w / 2,
                     qt->rect.y - qt->rect.h / 2);
  SDL_RenderDrawLine(ui->renderer, qt->rect.x - qt->rect.w / 2,
                     qt->rect.y - qt->rect.h / 2, qt->rect.x - qt->rect.w / 2,
                     qt->rect.y + qt->rect.h / 2);
  SDL_RenderDrawLine(ui->renderer, qt->rect.x + qt->rect.w / 2,
                     qt->rect.y - qt->rect.h / 2, qt->rect.x + qt->rect.w / 2,
                     qt->rect.y + qt->rect.h / 2);
  SDL_RenderDrawLine(ui->renderer, qt->rect.x - qt->rect.w / 2,
                     qt->rect.y + qt->rect.h / 2, qt->rect.x + qt->rect.w / 2,
                     qt->rect.y + qt->rect.h / 2);
  if (qt->children != NULL) {
    for (int i = 0; i < 4; i++) {
      quad_tree_draw(qt->children[i], ui);
    }
  }
}
