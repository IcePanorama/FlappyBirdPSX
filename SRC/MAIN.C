#include "game/gamemngr.h"
#include "input/controlr.h"
#include "sys/fb_bools.h"
#include "video/video.h"

#include <sys/types.h>
#include <libcd.h>

#ifdef DEBUG_BUILD
  #include <assert.h>
  #include <stdio.h>
#endif /* DEBUG_BUILD */

static int init_cd (void);

// TODO: make font a 1 bit texture.
int
main (void)
{
  init_cd ();
  v_init_video ();
  ctrl_init_controllers ();
  gm_init_game ();

  while (TRUE)
  {
    gm_update_game ();
    v_render_screen ();
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
