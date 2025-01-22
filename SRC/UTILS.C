#include "utils.h"
#include "game/gmstate.h"
#include "sys/fb_bools.h"
#include "sys/fb_defs.h"
#include "sys/fb_time.h"

#include <rand.h>

#ifdef DEBUG_BUILD
  #include <stdio.h>
#endif /* DEBUG_BUILD */

void
v2_convert_world_space_to_camera_space (Vec2_t *v2_input, Vec2_t *v2_output)
{
  v2_output->x =
    (v2_input->x >> (FB_WORLD_TO_CAMERA_SPACE_NUM_SHIFTS)) + ((FB_HALF_SCREEN_WIDTH));
  v2_output->y =
    (v2_input->y >> (FB_WORLD_TO_CAMERA_SPACE_NUM_SHIFTS)) + ((FB_HALF_SCREEN_HEIGHT));
}

void
u_seed_rng (void)
{
  static bool_t b_rng_seeded = (FALSE);
  uint16_t u16_seed;

  // FIXME: should this change really be happening here?
  gs_set_game_state (GSTATE_NORMAL);

  if (b_rng_seeded)  return;
  b_rng_seeded = TRUE;

  u16_seed = (TIME_GET_CURRENT_TIME());
#ifdef DEBUG_BUILD
  printf ("Rand seed: %d\n", u16_seed);
#endif /* DEBUG_BUILD */

  srand(u16_seed);
}
