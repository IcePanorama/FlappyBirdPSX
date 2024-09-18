#include "G_AABB.H"

int g_are_colliding(AABB_t *a, AABB_t *b) {
  return ((a->position.vx < b->position.vx + b->width) &&
          (a->position.vx + a->width > b->position.vx) &&
          (a->position.vy < b->position.vy + b->height) &&
          (a->position.vy + a->height > a->position.vy));
}

#ifdef WIREFRAME
void g_init_AABB_wireframe(AABB_t *a) {
  int i;

  for (i = 0; i < NUM_WIREFRAME_LNS; i++) {
    SetLineF2(&a->wireframe[i]);
    setRGB0(&a->wireframe[i], 255, 255, 255);
  }

  g_update_AABB_wireframe_xy(a);
}

void g_update_AABB_wireframe_xy(AABB_t *a) {
  long left_x;
  long right_x;
  long top_y;
  long bot_y;

  left_x = a->position.vx - (a->width >> 1);
  right_x = a->position.vx + (a->width >> 1);
  top_y = a->position.vy - (a->height >> 1);
  bot_y = a->position.vy + (a->height >> 1);

  /* Line 0 */
  setXY2(&a->wireframe[0], left_x, top_y, right_x, top_y);
  /* Line 1 */
  setXY2(&a->wireframe[1], right_x, top_y, left_x, bot_y);
  /* Line 2 */
  setXY2(&a->wireframe[2], left_x, bot_y, left_x, top_y);
  /* Line 3 */
  setXY2(&a->wireframe[3], right_x, top_y, right_x, bot_y);
  /* Line 4 */
  setXY2(&a->wireframe[4], right_x, bot_y, left_x, bot_y);
}
#endif /* WIREFRAME */
