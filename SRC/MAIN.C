#include "game/gamemngr.h"
#include "sys/fb_bools.h"
#include "video.h"

// TODO: finish randomized spawn height for pipes.
int
main (void)
{
  init_video_subsystem ();
  init_game ();

  while (TRUE)
  {
    update_game ();
    render_screen ();
  }

  destroy_game ();

  return 0;
}
