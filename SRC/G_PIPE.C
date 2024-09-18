#include <rand.h>
#include <stdio.h>

#include "FB_DEFS.H"
#include "G_PIPE.H"
#include "S_HBCNTR.H"

#define HALF_PIPE_WIDTH (8)
#define GAP_SIZE (60)
#define HALF_GAP_SIZE (GAP_SIZE >> 1)
#define GAP_PLUS_FIVE (GAP_SIZE + 5)

static void update_pipe_sprite_xy(Pipe_t *p);

// Just to make the code a bit cleaner.
enum PipeColShapeTypes_e {
  PIPE_TOP_COL_SHAPE,
  PIPE_BOT_COL_SHAPE,
  NUM_PIPE_COL_SHAPES
};

Pipe_t g_create_pipe(void) {
  Pipe_t p;

  SetPolyF4(&p.top_sprite);
  SetShadeTex(&p.top_sprite, 1);
  setRGB0(&p.top_sprite, 255, 0, 0);

  SetPolyF4(&p.bot_sprite);
  SetShadeTex(&p.bot_sprite, 1);
  setRGB0(&p.bot_sprite, 255, 0, 0);

  p.position.vx = SCREEN_WIDTH + PIPE_WIDTH;
  p.position.vy =
      rand() % (SCREEN_HEIGHT - 2 * (GAP_PLUS_FIVE)) + (GAP_PLUS_FIVE);

  p.col_shape[PIPE_TOP_COL_SHAPE].position.vx = p.position.vx;
  p.col_shape[PIPE_TOP_COL_SHAPE].width = HALF_PIPE_WIDTH << 1;
  p.col_shape[PIPE_TOP_COL_SHAPE].height = p.position.vy - HALF_GAP_SIZE;
  p.col_shape[PIPE_TOP_COL_SHAPE].position.vy =
      p.col_shape[PIPE_TOP_COL_SHAPE].height >> 1;

  p.col_shape[PIPE_BOT_COL_SHAPE].position.vx = p.position.vx;
  p.col_shape[PIPE_BOT_COL_SHAPE].width = HALF_PIPE_WIDTH << 1;
  p.col_shape[PIPE_BOT_COL_SHAPE].height =
      SCREEN_HEIGHT - (p.position.vy + HALF_GAP_SIZE);
  p.col_shape[PIPE_BOT_COL_SHAPE].position.vy =
      p.position.vy + HALF_GAP_SIZE +
      (p.col_shape[PIPE_BOT_COL_SHAPE].height >> 1);

#ifdef WIREFRAME
  g_init_AABB_wireframe(&p.col_shape[PIPE_TOP_COL_SHAPE]);
  g_init_AABB_wireframe(&p.col_shape[PIPE_BOT_COL_SHAPE]);
#endif /* WIREFRAME */

  update_pipe_sprite_xy(&p);

  return p;
}

void update_pipe_sprite_xy(Pipe_t *p) {
  short left_x;
  short right_x;
  short gap_start_y;
  short gap_end_y;

  left_x = p->position.vx - HALF_PIPE_WIDTH;
  right_x = p->position.vx + HALF_PIPE_WIDTH;
  gap_start_y = p->position.vy - HALF_GAP_SIZE;
  gap_end_y = p->position.vy + HALF_GAP_SIZE;

  setXY4(&p->top_sprite,
         //           x,           y
         left_x, 0,              // Top left
         right_x, 0,             // Top right
         left_x, gap_start_y,    // Bot left
         right_x, gap_start_y);  // Bot right

  setXY4(&p->bot_sprite,
         //           x,             y
         left_x, gap_end_y,        // Top left
         right_x, gap_end_y,       // Top right
         left_x, SCREEN_HEIGHT,    // Bot left
         right_x, SCREEN_HEIGHT);  // Bot right
}

void g_pipe_loop(Pipe_t *p) {
  p->position.vx -= 50 * s_delta_time() / 4096;
  p->col_shape[PIPE_TOP_COL_SHAPE].position.vx = p->position.vx;
  p->col_shape[PIPE_BOT_COL_SHAPE].position.vx = p->position.vx;

#ifdef WIREFRAME
  g_update_AABB_wireframe_xy(&p->col_shape[PIPE_TOP_COL_SHAPE]);
  g_update_AABB_wireframe_xy(&p->col_shape[PIPE_BOT_COL_SHAPE]);
#endif /* WIREFRAME */

  update_pipe_sprite_xy(p);
}
