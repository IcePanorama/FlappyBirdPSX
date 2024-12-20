#include "game/colmngr.h"
#include "compnts/colshape.h"
#include "compnts/physics.h"
#include "game_obj/entityid.h"
#include "game_obj/pipes.h"
#include "sys/fb_bools.h"
#include "sys/fb_defs.h"
#include "sys/fb_ints.h"

#ifdef DEBUG_BUILD
  #include <assert.h>
  #include <stdio.h>
#endif /* DEBUG_BUILD */

static bool_t x_pos_in_red_zone (int16_t x);

static PlayerEntity_t *player;

void
cm_init_collision_manager (PlayerEntity_t *p)
{
#ifdef DEBUG_BUILD
  assert(p != 0);
#endif /* DEBUG_BUILD */

  player = p;
}

void
cm_handle_collisions (void)
{
  uint8_t i;
  ColShapeCompnt_t *csc_pipe;

#ifdef DEBUG_BUILD
  assert(player != 0);
#endif /* DEBUG_BUILD */

  player->pcsc_col_shape = get_col_shape_with_id (player->u8_eid);

#ifdef DEBUG_BUILD
  assert(player->pcsc_col_shape != 0);
#endif /* DEBUG_BUILD */

  for (i = 0; i < u8_pc_num_physics; i++)
  {
    if (pc_physics_pool[i].u8_parent_id == EID_PLAYER_ID)
      continue;
    else if (x_pos_in_red_zone (pc_physics_pool[i].v2_position.x))
    {
      csc_pipe = get_col_shape_with_id(pc_physics_pool[i].u8_parent_id);

      // FIXME: Should this send a signal instead?
      if (csc_detect_collision (player->pcsc_col_shape, csc_pipe))
      {
        pe_kill_player (player);
      }
    }
  }
}

bool_t
x_pos_in_red_zone (int16_t x)
{
  const int16_t i16_RED_ZONE_X = ((player->u8_width >> 1) + (PIE_HALF_PIPE_WIDTH)) << (WORLD_TO_CAMERA_SPACE_NUM_SHIFTS);

  return -i16_RED_ZONE_X < x && x < i16_RED_ZONE_X;
}
