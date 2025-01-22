#include "game/score.h"
#include "game/gamemngr.h"
#include "game/pipemngr.h"
#include "game_obj/pipes.h"
#include "sys/fb_defs.h"
#include "video/uimanagr.h"

#ifdef DEBUG_BUILD
  #include <assert.h>
  #include <stdio.h>
#endif /* DEBUG_BUILD */

uint16_t s_current_score = 0;
//TODO: save and load this off of memory cards.
uint16_t s_high_score = 0;

static PlayerEntity_t *player;
static int16_t i16_green_area_start = 0;
//TODO: save and load this off of memory cards.
static uint16_t u16_2nd_high_score = 0;

void
s_init_scoring (PlayerEntity_t *p)
{
#ifdef DEBUG_BUILD
  assert(p != 0);
#endif /* DEBUG_BUILD */
  player = p;
  i16_green_area_start =
    -((player->u8_width + (PIE_HALF_PIPE_WIDTH)) << (FB_WORLD_TO_CAMERA_SPACE_NUM_SHIFTS));
  ui_set_game_over_coin_state (UICS_NO_COIN);
  ui_init_score_text (&s_current_score, &s_high_score);
  s_current_score = 0;
  ui_update_current_score_text ();
}

bool_t
s_in_green_area (int16_t x)
{
#ifdef DEBUG_BUILD
  assert(player != 0);
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
    s_current_score++;
    ui_update_current_score_text ();
    p->b_was_scored = (TRUE);
  }
}

void
s_process_high_scores (void)
{
  if (s_current_score > s_high_score)
  {
    u16_2nd_high_score = s_high_score;
    s_high_score = s_current_score;
    ui_update_high_score_text ();
    ui_set_game_over_coin_state (UICS_GOLD_COIN);
  }
  else if (s_current_score > u16_2nd_high_score)
  {
    u16_2nd_high_score = s_current_score;
    ui_set_game_over_coin_state (UICS_SILVER_COIN);
  }
}
