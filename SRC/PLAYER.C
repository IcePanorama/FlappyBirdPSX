#include "player.h"
#include "fb_defs.h"

#include <assert.h>
#include <stdio.h>

#define GRAVITY (-2)
#define JUMP_VELOCITY (8)

static void update_player_sprite_xy (Player_t *p);
static void player_die (Player_t *p);

Player_t
create_player_entity (uint16_t start_x, uint16_t start_y)
{
  Player_t p;

  p.is_alive = TRUE;

  p.position.x = start_x;
  p.position.y = start_y;

  p.velocity_y = 0;

  p.width = p.height = 16;

  //FIXME: how would we handle getting getting null in release builds?
  p.sprite_component = get_new_sprite_from_pool ();
#ifdef DEBUG_BUILD
  assert(p.sprite_component != 0);
#endif /* DEBUG_BUILD */
  init_sprite_w_rgb (&p.sprite_component->sprite, 0, 255, 0);
  update_player_sprite_xy (&p);

  //FIXME: how would we handle getting getting null in release builds?
  p.col_shape = get_new_col_shape_from_pool ();
#ifdef DEBUG_BUILD
  assert(p.col_shape != 0);
#endif /* DEBUG_BUILD */

  return p;
}

void
update_player_sprite_xy (Player_t *p)
{
  uint16_t half_width = (p->width >> 1);
  uint16_t half_height = (p->height >> 1);

  setXY4(&p->sprite_component->sprite,
         p->position.x - half_width, p->position.y + half_height,
         p->position.x + half_width, p->position.y + half_height,
         p->position.x - half_width, p->position.y - half_height,
         p->position.x + half_width, p->position.y - half_height);

  update_col_shape_xy_from_center (p->col_shape, p->position, p->width,
                                   p->height);
}

void
update_player_entity (Player_t *p)
{
  p->position.y -= p->velocity_y;
  if (p->position.y >= SCREEN_HEIGHT + 8)
  {
    player_die (p);
    return;
  }

  if (p->velocity_y > GRAVITY)
    p->velocity_y -= 1;
  else
    p->velocity_y = GRAVITY;

  update_player_sprite_xy (p);
}

void
player_die (Player_t *p)
{
  free_sprite (p->sprite_component);
  free_col_shape (p->col_shape);
  p->is_alive = FALSE;
}

void
player_jump (void *p)
{
  ((Player_t *)(p))->velocity_y = JUMP_VELOCITY;
}

#undef GRAVITY
#undef JUMP_VELOCITY
