#include "memcard.h"
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

static int init_cd (void);

int
main (void)
{
  if (mc_init () == -1)
    return -1;

  ctrl_init_controllers ();
  init_cd ();

  r_init_renderer ();
  gm_init_game ();

  while (TRUE)
    {
      gm_update_game ();
      r_render_screen ();
    }

  gm_destroy_game ();

  return 0;
}

int
init_cd (void)
{
#ifdef DEBUG_BUILD
  CdSetDebug (2);
  assert(CdInit() != 0);
#else /* DEBUG_BUILD */
  if (CdInit() == 0)
    return -1;
#endif /* DEBUG_BUILD */

  return 0;
}
