#include "game/gamemngr.h"
#include "compnts/physics.h"
#include "compnts/sprites.h"
#include "game_obj/player.h"
#include "input/cmdlist.h"
#include "input/controlr.h"

#define MAX_NUM_ENTITIES (32)

PlayerEntity_t player;

static void update_physics_compnts (Vec2_t *v2_output_pos);
static void update_sprites (Vec2_t *v2_input_pos);

void
init_game (void)
{
  ctrl_init_controllers ();
  player = create_player_entity ();
}

void
update_game (void)
{
  Vec2_t v2_entity_pos[MAX_NUM_ENTITIES] = {{0}};

  ctrl_handle_user_input (cmdl_command_lists[gs_curr_game_state],
                          (void *)&player);
  update_physics_compnts (v2_entity_pos);
  update_sprites (v2_entity_pos);
}

void
destroy_game (void)
{
  destroy_player_entity (&player);
}

void
update_physics_compnts (Vec2_t *v2_output_pos)
{
  uint8_t i;

  for (i = 0; i < u8_pc_num_physics; i++)
  {
    update_physics_compnt (&pc_physics_pool[i], &v2_output_pos[i]);
  }
}

void
update_sprites (Vec2_t *v2_input_pos)
{
  uint8_t i;

  for (i = 0; i < sp_num_sprites; i++)
  {
    (*sp_sprite_pool[i].update)(&sp_sprite_pool[i], &v2_input_pos[i]);
  }
}

#undef MAX_NUM_ENTITIES
