/* Don't touch! CC seems really particular about the order of these. */
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libetc.h>
/******************/

#include "compnts/sprites.h"
#include "sys/fb_ints.h"
#include "sys/fb_defs.h"
#include "video.h"

#ifdef DEBUG_BUILD
  #include <assert.h>
  #include <stdio.h>
#endif /* DEBUG_BUILD */

#define DIST_TO_SCREEN (512)
#define OT_MAX_LEN (4096)

typedef struct ScreenBuffer_s
{
  DRAWENV draw_env;
  DISPENV disp_env;
  u_long ordering_table[OT_MAX_LEN];
} ScreenBuffer_t;

static ScreenBuffer_t screen_buffer[2];

static void init_screen_buffers (ScreenBuffer_t *sb);

void
init_video_subsystem (void)
{
  //TODO: figure out how to handle NTSC/PAL differences elegantly
  SetVideoMode (0);
  GsInitGraph (SCREEN_WIDTH, SCREEN_HEIGHT, GsNONINTER|GsOFSGPU, 1, 0);

	FntLoad (960, 256);
	SetDumpFnt (FntOpen (0, 8, SCREEN_WIDTH, 64, 0, 512));
	SetGraphDebug (0);

	InitGeom ();  // Init GTE
	SetGeomOffset (SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	SetGeomScreen (DIST_TO_SCREEN);

  init_screen_buffers (screen_buffer);

	SetDispMask (1);
}

void
init_screen_buffers (ScreenBuffer_t *sb)
{
  //TODO: I may be able to afford 480i, given how simple FB is.
	SetDefDrawEnv (&sb[0].draw_env, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	SetDefDrawEnv (&sb[1].draw_env, 0, SCREEN_HEIGHT, SCREEN_WIDTH,
                 SCREEN_HEIGHT);
	SetDefDispEnv (&sb[0].disp_env, 0, SCREEN_HEIGHT, SCREEN_WIDTH,
                 SCREEN_HEIGHT);
	SetDefDispEnv (&sb[1].disp_env, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

  sb[0].draw_env.isbg = sb[1].draw_env.isbg = 1;
  setRGB0(&sb[0].draw_env, 100, 100, 100);
  setRGB0(&sb[1].draw_env, 100, 100, 100);
}

//FIXME: needs refactoring!
void
render_screen (void)
{
  uint8_t i;
  u_long ot_idx = 0;
  static ScreenBuffer_t *curr_sb = screen_buffer;
  curr_sb = (curr_sb == screen_buffer) ? screen_buffer + 1 : screen_buffer;

  ClearOTag (curr_sb->ordering_table, OT_MAX_LEN);

  for (i = 1; i < sp_num_sprites; i++)
  {
#ifdef DEBUG_BUILD
    assert(ot_idx < OT_MAX_LEN);
#endif /* DEBUG_BUILD */

    AddPrim (&curr_sb->ordering_table[ot_idx], &sp_sprite_pool[i].p4_sprite);
    ot_idx++;
  }

  /* Draw player on top. This assume's player is always sprite 0. */
  AddPrim (&curr_sb->ordering_table[ot_idx], &sp_sprite_pool[0].p4_sprite);
  ot_idx++;

  DrawSync (0);
  VSync (0);

  PutDrawEnv (&curr_sb->draw_env);
  PutDispEnv (&curr_sb->disp_env);

  ClearImage (&curr_sb->draw_env.clip, 100, 100, 100);

  DumpOTag (curr_sb->ordering_table);
  DrawOTag (curr_sb->ordering_table);

  FntFlush (-1);
}

#undef DIST_TO_SCREEN
#undef OT_MAX_LEN
