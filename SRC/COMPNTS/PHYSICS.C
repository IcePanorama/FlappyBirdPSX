#include "compnts/physics.h"

#ifdef DEBUG_BUILD
  #include <stdio.h>
  #include <assert.h>
#endif /* DEBUG_BUILD */

PhysicsCompnt_t pc_physics_pool[(PHYSICS_MAX_NUM_PHYSICS_COMP)] = {{0}};
uint8_t u8_pc_num_physics = 0;

void
create_new_physics_compnt (int8_t i8_id)
{
#ifdef DEBUG_BUILD
  assert((u8_pc_num_physics + 1) < PHYSICS_MAX_NUM_PHYSICS_COMP);
#endif /* DEBUG_BUILD */

  pc_physics_pool[u8_pc_num_physics].i8_parent_id = i8_id;
  u8_pc_num_physics++;
}

void
destroy_physics_compnt (int8_t i8_id)
{
  PhysicsCompnt_t pc_tmp;
  uint8_t i;
  uint8_t u8_idx = -1;

  for (i = 0; i < u8_pc_num_physics; i++)
  {
    if (pc_physics_pool[i].i8_parent_id == i8_id)
    {
      u8_idx = i;
      break;
    }
  }

#ifdef DEBUG_BUILD
  assert(u8_idx != (uint8_t)-1);
#endif /* DEBUG_BUILD */

  /* if the given sprite is last active sprite, swapping isn't necessary. */
  if (u8_idx != (u8_pc_num_physics - 1))
  {
    pc_tmp = pc_physics_pool[u8_idx];
    pc_physics_pool[u8_idx] = pc_physics_pool[u8_pc_num_physics - 1];
    pc_physics_pool[u8_pc_num_physics - 1] = pc_tmp;
  }

  u8_pc_num_physics--;
}

PhysicsCompnt_t *
get_physics_compnt_with_id (int8_t i8_id)
{
  uint8_t i;

  for (i = 0; i < u8_pc_num_physics; i++)
  {
    if (pc_physics_pool[i].i8_parent_id == i8_id)
    {
      return &pc_physics_pool[i];
    }
  }

#ifdef DEBUG_BUILD
  printf ("Physics component with id, %d, not found!\n", i8_id);
  assert(0);
#endif /* DEBUG_BUILD */

  return 0;
}

// LO: finish physics component update function
void
update_physics_compnt (PhysicsCompnt_t *pc, Vec2_t *v2_output_pos)
{
  pc->position.y -= pc->velocity;
  if (pc->position.y >= SCREEN_HEIGHT)
    pc->position.y = 0;

//TODO: rename to y_velocity
  if (pc->velocity > GRAVITY)
    ;
  pc->velocity -= GRAVITY;

}
