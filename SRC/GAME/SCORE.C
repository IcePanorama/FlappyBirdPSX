#include "game/score.h"
#include "game/gamemngr.h"
#include "game/pipemngr.h"
#include "game_obj/pipes.h"
#include "sys/fb_defs.h"

#ifdef DEBUG_BUILD
  #include <assert.h>
  #include <stdio.h>
#endif /* DEBUG_BUILD */

static PlayerEntity_t *player;
static int16_t i16_green_area_start = 0;

void
s_init_scoring (PlayerEntity_t *p)
{
#ifdef DEBUG_BUILD
  assert(p != 0);
#endif /* DEBUG_BUILD */
  player = p;
  i16_green_area_start = -((player->u8_width + (PIE_HALF_PIPE_WIDTH)) << (WORLD_TO_CAMERA_SPACE_NUM_SHIFTS));
}

bool_t
s_in_green_area (int16_t x)
{
#ifdef DEBUG_BUILD
  assert(player != 0);
  assert(i16_green_area_start != 0);
#endif /* DEBUG_BUILD */

  return x < i16_green_area_start;
}

void
s_process_scoring (uint8_t u8_eid)
{
  PipesEntity_t* p;
  if (u8_eid == 0) return;      // Ignore player entity
  if (u8_eid % 2 == 0) return;  // Ignore bottom pipes, each pair counts once!

  p = pm_get_pipe(u8_eid);
  if (!p->b_was_scored)
  {
    gm_increase_score ();
    p->b_was_scored = (TRUE);
  }
}
