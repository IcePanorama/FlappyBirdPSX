#include "game_obj/pipes.h"
#include "compnts/physics.h"
#include "compnts/sprites.h"
#include "game_obj/entityid.h"
#include "sys/fb_defs.h"
#include "utils.h"

#include <rand.h>

#ifdef DEBUG_BUILD
  #include <assert.h>
  #include <stdio.h>
#endif /* DEBUG_BUILD */

#define MAX_NUM_PIPES_VALUE (255)
#define PIPE_WIDTH (34)
#define HALF_PIPE_WIDTH (17)
#define MOVE_SPEED (30)
#define HALF_GAP_SIZE (40)
#define GAP_PLUS_FIVE (85)
#define OFF_SCREEN (\
  ((-HALF_SCREEN_WIDTH) - (HALF_PIPE_WIDTH)) \
  << (WORLD_TO_CAMERA_SPACE_NUM_SHIFTS))

static void init_pipes_physics_compnts (PipesEntity_t *pe, Vec2_t v2_out_pos[2],
                                        uint16_t u16_heights[2]);
static void init_pipes_sprite_compnts (PipesEntity_t *pe, uint16_t u16_heights[2]);

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

  // FIXME: we only care about the y value, this shouldn't be a v2!
  pe.v2_origin.y = rand() % 101 - 50;

  init_pipes_physics_compnts (&pe, v2_pos, u16_heights);
  init_pipes_sprite_compnts (&pe, u16_heights);

  /*
   *  FIXME: we absolutely must ensure that our `SpriteComponent_t` ptr isn't
   *  moved around between initialization and updating the sprite when a
   *  `PipeEntity_t` is first created. Ideally, the updating of sprite xy's
   *  and the deletion of a `SpriteComponent_t` should happen in completely
   *  separate parts of the game loop. Perhaps the physics components can tell
   *  the game manager once that they died, and that'll handle everything all at
   *  once somewhere in the update loop, after all of the other updates have
   *  finished.
   */
  for (i = 0; i < 2; i++)
  {
    if (pe.psc_sprite_compnts[i] != 0)
      pie_update_pipes_sprite_xy (pe.psc_sprite_compnts[i], &v2_pos[i]);
  }

  u8_num_pipes_created = (u8_num_pipes_created + 1) % MAX_NUM_PIPES_VALUE;
  return pe;
}

void
init_pipes_physics_compnts (PipesEntity_t *pe, Vec2_t v2_out_pos[2],
                            uint16_t u16_heights[2])
{
  uint8_t i;
  for (i = 0; i < 2; i++) // to init the top and bot with common data.
  {
    create_new_physics_compnt (pe->u8_eid + i);
    pe->ppc_physics_compnts[i] = get_physics_compnt_with_id(pe->u8_eid + i);

#ifdef DEBUG_BUILD
    //FIXME: handle recieving null component gracefully in release builds.
    assert(pe->ppc_physics_compnts[i] != 0)
#endif /* DEBUG_BUILD */

    // Default starting x pos is offscreen, on the right
    pe->ppc_physics_compnts[i]->v2_position.x =
      ((HALF_SCREEN_WIDTH) + (HALF_PIPE_WIDTH));
    pe->ppc_physics_compnts[i]->v2_position.x <<=
      (WORLD_TO_CAMERA_SPACE_NUM_SHIFTS);

    v2_out_pos[i].x = pe->ppc_physics_compnts[i]->v2_position.x;

    pe->ppc_physics_compnts[i]->v2_velocity.x = -MOVE_SPEED;
    pe->ppc_physics_compnts[i]->v2_velocity.y = 0;

    pe->ppc_physics_compnts[i]->b_use_gravity = FALSE;
  }

  u16_heights[0] = (pe->v2_origin.y - (HALF_GAP_SIZE)) + (HALF_SCREEN_HEIGHT);
  u16_heights[1] = (HALF_SCREEN_HEIGHT) - (pe->v2_origin.y + (HALF_GAP_SIZE));

  /* Calculate top pipe's y pos. */
  pe->ppc_physics_compnts[0]->v2_position.y =
    pe->v2_origin.y - (HALF_GAP_SIZE) - (u16_heights[0] >> 1);
  pe->ppc_physics_compnts[0]->v2_position.y <<=
    (WORLD_TO_CAMERA_SPACE_NUM_SHIFTS);

  v2_out_pos[0].y = pe->ppc_physics_compnts[0]->v2_position.y;
  pe->ppc_physics_compnts[0] = 0;

  /* Calculate bot pipe's y pos. */
  pe->ppc_physics_compnts[1]->v2_position.y =
    pe->v2_origin.y + (HALF_GAP_SIZE) + (u16_heights[1] >> 1);
  pe->ppc_physics_compnts[1]->v2_position.y <<=
    (WORLD_TO_CAMERA_SPACE_NUM_SHIFTS);

  v2_out_pos[1].y = pe->ppc_physics_compnts[1]->v2_position.y;
  pe->ppc_physics_compnts[1] = 0;
}

void
init_pipes_sprite_compnts (PipesEntity_t *pe, uint16_t u16_heights[2])
{
  uint8_t i;
  for (i = 0; i < 2; i++)
  {
    create_new_sprite (pe->u8_eid + i);
    pe->psc_sprite_compnts[i] = get_sprite_with_id(pe->u8_eid + i);

#ifdef DEBUG_BUILD
    assert(pe->psc_sprite_compnts[i] != 0);
#endif /* DEBUG_BUILD */

    pe->psc_sprite_compnts[i]->u8_width = (PIPE_WIDTH);
    pe->psc_sprite_compnts[i]->u8_height = u16_heights[i];

    SetPolyF4 (&pe->psc_sprite_compnts[i]->p4_sprite);
    SetShadeTex (&pe->psc_sprite_compnts[i]->p4_sprite, 1);
    setRGB0(&pe->psc_sprite_compnts[i]->p4_sprite, 255, 0, 0);

    pe->psc_sprite_compnts[i]->update = pie_update_pipes_sprite_xy;
  }
}

void
pie_destroy_pipes_entity (PipesEntity_t *pe)
{
  uint8_t i;
  for (i = 0; i < 2; i++)
  {
    destroy_sprite (pe->u8_eid + i);
    destroy_physics_compnt (pe->u8_eid + i);
  }
}

void
pie_update_pipes_sprite_xy (SpriteCompnt_t *sc, Vec2_t *v2_pos)
{
  Vec2_t v2_cs_pos;  // camera-space position
  uint8_t u8_half_height;
  uint16_t u16_left_x;
  uint16_t u16_right_x;
  uint16_t u16_top_y;
  uint16_t u16_bot_y;

  if (sc == 0 || v2_pos == 0)
    return;

  u8_half_height = sc->u8_height >> 1;

  v2_convert_world_space_to_camera_space (v2_pos, &v2_cs_pos);

  u16_left_x  = v2_cs_pos.x - (HALF_PIPE_WIDTH);
  u16_right_x = v2_cs_pos.x + (HALF_PIPE_WIDTH);
  u16_top_y   = v2_cs_pos.y - u8_half_height;
  u16_bot_y   = v2_cs_pos.y + u8_half_height;

  setXY4(&sc->p4_sprite,
          u16_left_x, u16_top_y,
         u16_right_x, u16_top_y,
          u16_left_x, u16_bot_y,
         u16_right_x, u16_bot_y);
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

#undef MAX_NUM_PIPES_VALUE
#undef PIPE_WIDTH
#undef HALF_PIPE_WIDTH
#undef MOVE_SPEED
#undef HALF_GAP_SIZE
