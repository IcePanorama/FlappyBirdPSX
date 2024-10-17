#include "game_obj/player.h"
#include "compnts/sprites.h"
#include "compnts/physics.h"
#include "sys/fb_defs.h"

#ifdef DEBUG_BUILD
  #include <stdio.h>
  #include <assert.h>
#endif /* DEBUG_BUILD */

static void init_player_physics_compnt (PlayerEntity_t *pe,
                                        Vec2_t *v2_output_pos);
static void init_player_sprite_compnt (PlayerEntity_t *pe);

PlayerEntity_t
create_player_entity (void)
{
  PlayerEntity_t pe;
  Vec2_t v2_pos;

  pe.eid_id = EID_PLAYER_ID;
  pe.u8_width = pe.u8_height = 16;

  init_player_physics_compnt (&pe, &v2_pos);
  pe.ppc_physics_compnt = 0; // set this to NULL when we're done w/ it.

  init_player_sprite_compnt (&pe);
  update_player_sprite_xy (&pe, &v2_pos);
  pe.psc_sprite_compnt = 0;  // set this to NULL when we're done w/ it.

  return pe;
}

//TODO: make macro version of this function.
void
init_player_physics_compnt (PlayerEntity_t *pe, Vec2_t *v2_output_pos)
{
  create_new_physics_compnt (pe->eid_id);
  pe->ppc_physics_compnt = get_physics_compnt_with_id(pe->eid_id);

#ifdef DEBUG_BUILD
  assert(pe->ppc_physics_compnt != 0);
#endif /* DEBUG_BUILD */

  pe->ppc_physics_compnt->v2_position.x = 0;
  pe->ppc_physics_compnt->v2_position.y = 0;
  pe->ppc_physics_compnt->i32_velocity = 0;

  v2_output_pos->x = pe->ppc_physics_compnt->v2_position.x;
  v2_output_pos->y = pe->ppc_physics_compnt->v2_position.y;
}

//TODO: make macro version of this function.
void
init_player_sprite_compnt (PlayerEntity_t *pe)
{
  create_new_sprite (pe->eid_id);
  pe->psc_sprite_compnt = get_sprite_with_id(pe->eid_id);

#ifdef DEBUG_BUILD
  assert(pe->psc_sprite_compnt != 0);
#endif /* DEBUG_BUILD */

  SetPolyF4 (&pe->psc_sprite_compnt->p4_sprite);
  SetShadeTex (&pe->psc_sprite_compnt->p4_sprite, 1);
  setRGB0(&pe->psc_sprite_compnt->p4_sprite, 0, 255, 0);
}

void
update_player_sprite_xy (PlayerEntity_t *pe, Vec2_t *v2_pos)
{
  Vec2_t v2_cs_pos;  // camera-space position
  const uint8_t U8_HALF_WIDTH = pe->u8_width >> 1;
  const uint8_t U8_HALF_HEIGHT = pe->u8_height >> 1;
  uint16_t u16_left_x;
  uint16_t u16_right_x;
  uint16_t u16_top_y;
  uint16_t u16_bot_y;

  if (pe == 0 || pe->psc_sprite_compnt == 0)
    return;

  // TODO: move this into a separate function
  // FIXME: player x pos should be constant
  // transform from world (origin @ center) to camera space (origin @ top-left)
  v2_cs_pos.x = v2_pos->x + (HALF_SCREEN_WIDTH);
  v2_cs_pos.y = v2_pos->y + (HALF_SCREEN_HEIGHT);

  u16_left_x  = v2_cs_pos.x - U8_HALF_WIDTH;
  u16_right_x = v2_cs_pos.x + U8_HALF_WIDTH;
  u16_top_y   = v2_cs_pos.y - U8_HALF_HEIGHT;
  u16_bot_y   = v2_cs_pos.y + U8_HALF_HEIGHT;

  setXY4(&pe->psc_sprite_compnt->p4_sprite,
          u16_left_x, u16_top_y,
         u16_right_x, u16_top_y,
          u16_left_x, u16_bot_y,
         u16_right_x, u16_bot_y);
}

void
destroy_player_entity (PlayerEntity_t *pe)
{
  destroy_sprite (pe->eid_id);
}
