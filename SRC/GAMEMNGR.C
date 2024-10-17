#include "gamemngr.h"
#include "game_obj/player.h"

#define MAX_NUM_ENTITIES (32)

//TODO: try making this static
PlayerEntity_t player;

static void update_physics_compnts (void);

void
init_game (void)
{
  player = create_player_entity ();
}

void
update_game (void)
{
//    handle user input
//    handle physics
  update_physics_compnts (void);
//    update sprites
}

void
destroy_game (void)
{
  destroy_player_entity (&player);
}

void
update_physics_compnts (void)
{
  static Vec2_t v2_entity_pos[MAX_NUM_ENTITIES] = {{0}};
  uint8_t i;

  for (i = 0; i < u8_pc_num_physics; i++)
  {
    update_physics_compnt (&ph_physics_pool[i], &v2_entity_pos[i]);
  }
}
