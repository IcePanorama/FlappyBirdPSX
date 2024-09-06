#include "G_PLAYER.H"
#include "V_VIDEO.H"

#include <stdio.h>

static ScreenBuffer_t screen_buffer[2];

int
main (void)
{
  Player_t player;
  //POLY_F4 sprite;

  init_video_subsystem (screen_buffer);

  SetPolyF4 (&player.sprite);
  SetShadeTex (&player.sprite, 1);
  setRGB0(&player.sprite, 0, 255, 0);

  /*
   *  Currently not accounting for sprite width/height, but that'll be easily
   *  fixable once we switch to using a `SPRT_16` later.
   */
  setXY4(&player.sprite, (SCREEN_WIDTH / 2) - 8, (SCREEN_HEIGHT / 2) + 8,
                         (SCREEN_WIDTH / 2) + 8, (SCREEN_HEIGHT / 2) + 8,
                         (SCREEN_WIDTH / 2) - 8, (SCREEN_HEIGHT / 2) - 8,
                         (SCREEN_WIDTH / 2) + 8, (SCREEN_HEIGHT / 2) - 8);

  while (1)
  {
    display_video (screen_buffer, 0);//&player.sprite);
  }

  return 0;
}
