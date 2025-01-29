#include "game/gamemngr.h"
#include "input/controlr.h"
#include "sys/fb_bools.h"
#include "video/renderer.h"

#include <sys/types.h>
#include <libcd.h>

#ifdef DEBUG_BUILD
  #include <assert.h>
  #include <stdio.h>
#endif /* DEBUG_BUILD */

#define MS_PER_UPDATE (17)

//tmp
#include "libetc.h"

void tick_clock (void);
static int init_cd (void);

static uint32_t clock_ticks = 0; // 1 tick = 17 MS

int
main (void)
{
  GameState_t curr_game_state;
  uint32_t current, previous, elapsed, lag;

  init_cd ();
  r_init_renderer ();
  VSyncCallback (tick_clock);

  ctrl_init_controllers ();
  gm_init_game ();

  previous = clock_ticks;
  lag = 0;
  while (TRUE)
  {
    current = clock_ticks;
    elapsed = (current >= previous) ? current - previous : current + (0xFFFF) - previous;
    previous = current;
    lag += elapsed;
//    printf ("ticks: %d, elapsed: %d (lag: %d)\n", current, elapsed, lag);

    curr_game_state = gs_get_curr_game_state ();
    ctrl_handle_user_input (cmdl_command_lists[curr_game_state], (void *)&player);

    while (lag >= (MS_PER_UPDATE))
    {
      gm_update_game ();
      lag -= (MS_PER_UPDATE);
    }

    r_render_screen ((lag * 4096) / (MS_PER_UPDATE));
  }

  gm_destroy_game ();

  return 0;
}

int
init_cd (void)
{
#ifdef DEBUG_BUILD
  CdSetDebug (3);
  assert(CdInit() != 0);
#else /* DEBUG_BUILD */
  if (CdInit() == 0)
    return -1;
#endif /* DEBUG_BUILD */

  return 0;
}

void
tick_clock (void)
{
  clock_ticks++;
}
