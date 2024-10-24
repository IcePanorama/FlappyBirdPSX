#include "game/gamemngr.h"
#include "sys/fb_bools.h"
#include "video.h"

/* FIXME: remove, tmp for testing. */
#include "game/signals.h"
#include "game_obj/pipes.h"

#include <stdio.h>
/***********************************/

// TODO: create pipe manager, then we'll be all finished with the rewrite.
int
main (void)
{
  PipesEntity_t p;
  bool_t b_alive = TRUE;
  Signal_t msg;

  init_video_subsystem ();
  init_game ();

  p = pie_create_pipes_entity ();

  while (TRUE)
  {
    update_game ();

    /*
     *  FIXME: this is HORRIBLY inefficient, and we aren't even dealing with
     *  multiple sets of pipes yet.
     */
    msg = si_check_inbox (p.u8_eid);
    //    printf("%d == 1 ? \n", msg);
    if (b_alive && msg == SIG_OFF_SCREEN_LEFT)
    {
      b_alive = FALSE;
      pie_destroy_pipes_entity (&p);
    }

    render_screen ();
  }

  destroy_game ();

  return 0;
}
