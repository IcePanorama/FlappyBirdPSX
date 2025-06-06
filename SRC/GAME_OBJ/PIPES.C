/** TODO: adjust spacing between pairs of pipes. */
#include "game_obj/pipes.h"
#include "compnts/colshape.h"
#include "compnts/physics.h"
#include "compnts/sprites.h"
#include "game_obj/entityid.h"
#include "sys/fb_defs.h"
#include "video/textlkup.h"
#include "utils.h"

#include <rand.h>

#ifdef DEBUG_BUILD
  #include <assert.h>
  #include <stdio.h>
#endif /* DEBUG_BUILD */

#define MAX_NUM_PIPES_VALUE (255)
#define HALF_GAP_SIZE (35)
#define PIPE_HEAD_SIZE (15)

#define RAND_ORIGIN_MIN   \
  (-(FB_HALF_SCREEN_HEIGHT) + (HALF_GAP_SIZE) + ((PIPE_HEAD_SIZE) << 1))
#define RAND_ORIGIN_MAX   \
  ((FB_HALF_SCREEN_HEIGHT) - (FB_FOREGROUND_HEIGHT) - (HALF_GAP_SIZE) - (PIPE_HEAD_SIZE))
#define RAND_ORIGIN_RANGE ((RAND_ORIGIN_MAX) - (RAND_ORIGIN_MIN) + 1)

#define OFF_SCREEN (\
  ((-(FB_HALF_SCREEN_WIDTH)) - (PIE_HALF_PIPE_WIDTH)) \
  << (FB_WORLD_TO_CAMERA_SPACE_NUM_SHIFTS))

static void init_pipes_physics_compnts (PipesEntity_t *pe, Vec2_t v2_out_pos[2],
                                        uint16_t u16_heights[2]);
static void init_pipes_sprite_compnts (PipesEntity_t *pe,
                                       uint16_t u16_heights[2]);

PipesEntity_t
pie_create_pipes_entity (void)
{
  PipesEntity_t pe;
  Vec2_t v2_pos[2];
  uint16_t u16_heights[2];
  uint8_t i;
  static uint8_t u8_num_pipes_created = 0;

  /* u8_eid + 0 = top pipe, eid_id + 1 = bot pipe */
  pe.u8_eid = EID_PIPES_ID + (u8_num_pipes_created << 1);

  //FIXME: we only care about the y value, this shouldn't be a v2!
  pe.v2_origin.y = rand() % RAND_ORIGIN_RANGE + RAND_ORIGIN_MIN;

  init_pipes_physics_compnts (&pe, v2_pos, u16_heights);

  init_pipes_sprite_compnts (&pe, u16_heights);

  for (i = 0; i < 2; i++)
  {
    sc_update_sprite_xy (pe.psc_sprite_compnts[i], &v2_pos[i]);
    pe.psc_sprite_compnts[i] = 0;  // set this to NULL when we're done w/ it.

    pe.pcsc_col_shape_compnts[i] =
      csc_create_new_col_shape (pe.u8_eid + i, (PIE_PIPE_WIDTH),
      u16_heights[i]);
    csc_update_col_shape (pe.pcsc_col_shape_compnts[i], &v2_pos[i]);
    pe.pcsc_col_shape_compnts[i] = 0;
  }

  pe.b_was_scored = FALSE;
  u8_num_pipes_created = (u8_num_pipes_created + 1) % (MAX_NUM_PIPES_VALUE);
  return pe;
}

void
init_pipes_physics_compnts (PipesEntity_t *pe, Vec2_t v2_out_pos[2],
                            uint16_t u16_heights[2])
{
  uint8_t i;
  for (i = 0; i < 2; i++) // to init the top and bot with common data.
  {
    pe->ppc_physics_compnts[i] = pc_create_new_physics_compnt (pe->u8_eid + i);

    // Default starting x pos is offscreen, on the right
    pe->ppc_physics_compnts[i]->v2_position.x =
      (((FB_HALF_SCREEN_WIDTH)) + (PIE_HALF_PIPE_WIDTH));
    pe->ppc_physics_compnts[i]->v2_position.x <<=
      (FB_WORLD_TO_CAMERA_SPACE_NUM_SHIFTS);

    v2_out_pos[i].x = pe->ppc_physics_compnts[i]->v2_position.x;

    pe->ppc_physics_compnts[i]->v2_velocity.x = -(FB_MOVEMENT_SPEED);
    pe->ppc_physics_compnts[i]->v2_velocity.y = 0;

    pe->ppc_physics_compnts[i]->b_use_gravity = FALSE;
  }

  u16_heights[0] = (pe->v2_origin.y - (HALF_GAP_SIZE)) + ((FB_HALF_SCREEN_HEIGHT));
  u16_heights[1] =
    (FB_HALF_SCREEN_HEIGHT) - (pe->v2_origin.y + (HALF_GAP_SIZE)) - ((FB_FOREGROUND_HEIGHT) - 1);

  /* Calculate top pipe's y pos. */
  pe->ppc_physics_compnts[0]->v2_position.y =
    pe->v2_origin.y - (HALF_GAP_SIZE) - (u16_heights[0] >> 1);
  pe->ppc_physics_compnts[0]->v2_position.y <<=
    (FB_WORLD_TO_CAMERA_SPACE_NUM_SHIFTS);

  v2_out_pos[0].y = pe->ppc_physics_compnts[0]->v2_position.y;
  pe->ppc_physics_compnts[0] = 0;

  /* Calculate bot pipe's y pos. */
  pe->ppc_physics_compnts[1]->v2_position.y =
    pe->v2_origin.y + (HALF_GAP_SIZE) + (u16_heights[1] >> 1);
  pe->ppc_physics_compnts[1]->v2_position.y <<=
    (FB_WORLD_TO_CAMERA_SPACE_NUM_SHIFTS);

  v2_out_pos[1].y = pe->ppc_physics_compnts[1]->v2_position.y;
  pe->ppc_physics_compnts[1] = 0;
}

#define PIPE_TEXTURE_HEIGHT (128)

void
init_pipes_sprite_compnts (PipesEntity_t *pe, uint16_t u16_heights[2])
{
  uint8_t i;

  for (i = 0; i < 2; i++)
  {
    pe->psc_sprite_compnts[i] =
      sc_create_new_sprite (&sprite_pools[TID_GAME_OBJ_TEXTURE], pe->u8_eid + i);
#ifdef DEBUG_BUILD
    assert(pe->psc_sprite_compnts[i] != 0);
#endif /* DEBUG_BUILD */

    if (i == 1)  // bot
      setUV0(&pe->psc_sprite_compnts[i]->sprite, (PIE_PIPE_WIDTH), 0);
    else  // top
      setUV0(&pe->psc_sprite_compnts[i]->sprite, 0,
             (PIPE_TEXTURE_HEIGHT) - u16_heights[i]);

    pe->psc_sprite_compnts[i]->u8_width = (PIE_PIPE_WIDTH);
    pe->psc_sprite_compnts[i]->u8_height = u16_heights[i];

    setWH(&pe->psc_sprite_compnts[i]->sprite, (PIE_PIPE_WIDTH), u16_heights[i]);
    pe->psc_sprite_compnts[i] = 0;  // set this to NULL when we're done w/ it.
  }
}

void
pie_destroy_pipes_entity (PipesEntity_t *pe)
{
  uint8_t i;
  for (i = 0; i < 2; i++)
  {
    csc_destroy_col_shape (pe->u8_eid + i);
    destroy_sprite (&sprite_pools[TID_GAME_OBJ_TEXTURE], pe->u8_eid + i);
    destroy_physics_compnt (pe->u8_eid + i);
  }
}

bool_t
pie_should_be_destroyed (PipesEntity_t *pe)
{
  if (pe == 0)
    return FALSE;

  pe->ppc_physics_compnts[0] = get_physics_compnt_with_id(pe->u8_eid);
  if (pe->ppc_physics_compnts[0] == 0)
    return FALSE;

  return pe->ppc_physics_compnts[0]->v2_position.x <= (OFF_SCREEN);
}
