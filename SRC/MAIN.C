#include "game/gamemngr.h"
#include "input/controlr.h"
#include "sys/fb_bools.h"
#include "video/video.h"

int
main (void)
{
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
