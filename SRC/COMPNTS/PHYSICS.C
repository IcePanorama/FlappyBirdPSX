#include "compnts/physics.h"
#include "game/score.h"
#include "game/signals.h"
#include "sys/fb_defs.h"

#ifdef DEBUG_BUILD
  #include <stdio.h>
  #include <assert.h>
#endif /* DEBUG_BUILD */

#define GRAVITY (4) // approx. for 9.8 / 2, in m/s^(2)

/** For position-related signals. */
#define BUFFER_ZONE_SIZE (64) // No sprite should be larger than 64x64.
#define BELOW_SCREEN_Y \
  ((((FB_HALF_SCREEN_HEIGHT)) + (BUFFER_ZONE_SIZE)) \
  << (FB_WORLD_TO_CAMERA_SPACE_NUM_SHIFTS))
#define ABOVE_SCREEN_Y (-(BELOW_SCREEN_Y))
#define OFF_SCREEN_RIGHT \
  ((((FB_HALF_SCREEN_WIDTH)) + (BUFFER_ZONE_SIZE)) \
  << (FB_WORLD_TO_CAMERA_SPACE_NUM_SHIFTS))
#define OFF_SCREEN_LEFT (-(OFF_SCREEN_RIGHT))
/**********************************/

PhysicsCompnt_t pc_physics_pool[(PHYSICS_MAX_NUM_PHYSICS_COMP)];// = {{0}};
uint8_t u8_pc_num_physics = 0;

static void manage_screen_position_signals (PhysicsCompnt_t *pc);


void
pc_init_physics_compnt_pool (void)
{
  memset (pc_physics_pool, 0,
          sizeof (PhysicsCompnt_t) * PHYSICS_MAX_NUM_PHYSICS_COMP);
  u8_pc_num_physics = 0;
}

PhysicsCompnt_t *
pc_create_new_physics_compnt (int8_t i8_id)
{
#ifdef DEBUG_BUILD
  assert((u8_pc_num_physics + 1) < PHYSICS_MAX_NUM_PHYSICS_COMP);
#endif /* DEBUG_BUILD */

  pc_physics_pool[u8_pc_num_physics].u8_parent_id = i8_id;
  u8_pc_num_physics++;
  return &pc_physics_pool[u8_pc_num_physics - 1];
}

void
destroy_physics_compnt (int8_t i8_id)
{
  PhysicsCompnt_t pc_tmp;
  uint8_t i;
  uint8_t u8_idx = -1;

  for (i = 0; i < u8_pc_num_physics; i++)
  {
    if (pc_physics_pool[i].u8_parent_id == i8_id)
    {
      u8_idx = i;
      break;
    }
  }

#ifdef DEBUG_BUILD
  assert(u8_idx != (uint8_t)-1);
#endif /* DEBUG_BUILD */

  /* if the given sprite is the last active sprite, swapping isn't necessary. */
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
    if (pc_physics_pool[i].u8_parent_id == i8_id)
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

void
update_physics_compnt (PhysicsCompnt_t *pc, Vec2_t *v2_output_pos)
{
  pc->v2_position.y += -pc->v2_velocity.y;

  if (pc->b_use_gravity)
  {
    if (pc->v2_velocity.y > 0)
       pc->v2_velocity.y -= (1 << FB_WORLD_TO_CAMERA_SPACE_NUM_SHIFTS);
    else
      pc->v2_velocity.y -= (GRAVITY);
  }

  pc->v2_position.x += pc->v2_velocity.x;

  if (s_in_green_area (pc->v2_position.x))
    s_process_scoring (pc->u8_parent_id);

  v2_output_pos->x = pc->v2_position.x;
  v2_output_pos->y = pc->v2_position.y;

  manage_screen_position_signals (pc);
}

void
manage_screen_position_signals (PhysicsCompnt_t *pc)
{
  if (pc->v2_position.x < (OFF_SCREEN_LEFT))
    si_send_signal(pc->u8_parent_id, SIG_OFF_SCREEN_LEFT);
  else if (pc->v2_position.x > OFF_SCREEN_RIGHT)
    si_send_signal(pc->u8_parent_id, (SIG_OFF_SCREEN_RIGHT));
  else if (pc->v2_position.y > (BELOW_SCREEN_Y))
    si_send_signal(pc->u8_parent_id, SIG_BELOW_SCREEN);
  else if (pc->v2_position.y < (ABOVE_SCREEN_Y))
    si_send_signal(pc->u8_parent_id, SIG_ABOVE_SCREEN);
}
