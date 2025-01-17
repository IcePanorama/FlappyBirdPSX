/* Do not touch! CC seems really particular about the order of these. */
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libetc.h>
/******************/

#include "video/scrbuff.h"
#include "sys/fb_defs.h"

ScreenBuffer_t screen_buffers[2];

void
sb_init_screen_buffers (void)
{
  //TODO: I may be able to afford 480i, given how simple FB is.
	SetDefDrawEnv (&screen_buffers[0].draw_env, 0, 0, (FB_SCREEN_WIDTH),
                 (FB_SCREEN_HEIGHT));
	SetDefDrawEnv (&screen_buffers[1].draw_env, 0, (FB_SCREEN_HEIGHT), (FB_SCREEN_WIDTH),
                 (FB_SCREEN_HEIGHT));
	SetDefDispEnv (&screen_buffers[0].disp_env, 0, (FB_SCREEN_HEIGHT), (FB_SCREEN_WIDTH),
                 (FB_SCREEN_HEIGHT));
	SetDefDispEnv (&screen_buffers[1].disp_env, 0, 0, (FB_SCREEN_WIDTH),
                 (FB_SCREEN_HEIGHT));

  screen_buffers[0].draw_env.isbg = screen_buffers[1].draw_env.isbg = 1;
  setRGB0(&screen_buffers[0].draw_env, 0, 0, 0);
  setRGB0(&screen_buffers[1].draw_env, 0, 0, 0);
}
