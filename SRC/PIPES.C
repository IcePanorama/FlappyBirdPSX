#include "pipes.h"
#include "fb_defs.h"

#include <rand.h>

#define HALF_PIPE_WIDTH (16)
#define HALF_GAP_SIZE (35)
#define GAP_PLUS_FIVE (75)  // Needed for randomly setting opening y pos later
#define PIPE_MOVE_SPEED (2)

static void update_pipes_sprite_xy (Pipes_t *p);
static void destroy_pipes_entity (Pipes_t *p);

Pipes_t
create_pipes_entity ()
{
  Pipes_t p;

  p.active = TRUE;

  p.position.x = SCREEN_WIDTH + HALF_PIPE_WIDTH;
  p.position.y = \
    rand() % (SCREEN_HEIGHT - 2 * (GAP_PLUS_FIVE)) + (GAP_PLUS_FIVE);

  //TODO: how would we handle getting getting null in release builds?
  p.top_sprite_component = get_new_sprite_from_pool ();
  p.bot_sprite_component = get_new_sprite_from_pool ();

  init_sprite_w_rgb (&p.top_sprite_component->sprite, 255, 0, 0);
  init_sprite_w_rgb (&p.bot_sprite_component->sprite, 255, 0, 0);

  //TODO: how would we handle getting getting null in release builds?
  p.top_col_shape = get_new_col_shape_from_pool ();
  p.bot_col_shape = get_new_col_shape_from_pool ();

  update_pipes_sprite_xy (&p);

  return p;
}

void
update_pipes_sprite_xy (Pipes_t *p)
{
  uint16_t u16_left_x;
  uint16_t u16_right_x;
  uint16_t u16_gap_start_y;
  uint16_t u16_gap_end_y;

  u16_left_x      = p->position.x - HALF_PIPE_WIDTH;
  u16_right_x     = p->position.x + HALF_PIPE_WIDTH;
  u16_gap_start_y = p->position.y - HALF_GAP_SIZE;
  u16_gap_end_y   = p->position.y + HALF_GAP_SIZE;

  setXY4(&p->top_sprite_component->sprite,
          u16_left_x,               0,
         u16_right_x,               0,
          u16_left_x, u16_gap_start_y,
         u16_right_x, u16_gap_start_y);

/*
  update_col_shape_xy_from_center (p->top_col_shape, p->position,
                                   (HALF_PIPE_WIDTH << 1),
                                   SCREEN_HEIGHT);
*/

  update_col_shape_xy_directly (p->top_col_shape, u16_left_x, u16_right_x, 0,
                                u16_gap_start_y);

  setXY4(&p->bot_sprite_component->sprite,
          u16_left_x, u16_gap_end_y,
         u16_right_x, u16_gap_end_y,
          u16_left_x, SCREEN_HEIGHT,
         u16_right_x, SCREEN_HEIGHT);

  update_col_shape_xy_directly (p->bot_col_shape, u16_left_x, u16_right_x,
                                u16_gap_end_y, SCREEN_HEIGHT);
/*
*/
}

void
update_pipes_entity (Pipes_t *p)
{
  if (p->position.x < -HALF_PIPE_WIDTH) // off-screen
  {
    destroy_pipes_entity (p);
    return;
  }

  p->position.x -= PIPE_MOVE_SPEED;
  update_pipes_sprite_xy (p);
}

void
destroy_pipes_entity (Pipes_t *p)
{
  p->active = FALSE;
  free_sprite (p->top_sprite_component);
  free_sprite (p->bot_sprite_component);
  free_col_shape (p->top_col_shape);
  free_col_shape (p->bot_col_shape);
}

#undef HALF_PIPE_WIDTH
#undef HALF_GAP_SIZE
#undef GAP_PLUS_FIVE
#undef PIPE_MOVE_SPEED
