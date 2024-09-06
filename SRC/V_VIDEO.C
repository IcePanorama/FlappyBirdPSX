#include "V_VIDEO.H"

#include <libetc.h>
#include <libgs.h>
#include <stdio.h>

#define DIST_TO_SCREEN (512)

void
init_video_subsystem (ScreenBuffer_t *sb)
{
  // FIXME: need to determine NTSC/PAL and make changes accordingly
  SetVideoMode (0);
  GsInitGraph (SCREEN_WIDTH, SCREEN_HEIGHT, GsNONINTER | GsOFSGPU, 1, 0);

  FntLoad (960, 256);
  SetDumpFnt (FntOpen (0, 8, SCREEN_WIDTH, 64, 0, 512));
  SetGraphDebug (0);

  InitGeom (); // Init GTE
  SetGeomOffset (SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  SetGeomScreen (DIST_TO_SCREEN);

  SetDefDrawEnv (&sb[0].draw_env, 0, 0, 320, 240);
  SetDefDrawEnv (&sb[1].draw_env, 0, 240, 320, 240);
  SetDefDispEnv (&sb[0].disp_env, 0, 240, 320, 240);
  SetDefDispEnv (&sb[1].disp_env, 0, 0, 320, 240);

  sb[0].draw_env.isbg = sb[1].draw_env.isbg = 1;
  setRGB0 (&sb[0].draw_env, 0, 0, 0);
  setRGB0 (&sb[1].draw_env, 0, 0, 0);

  // SetBackColor (0, 0, 0);  // Ambient color, set to black for the time being

  SetDispMask (1);
}

void
display_video (ScreenBuffer_t *sb, POLY_F4 *sprite)
{
  static ScreenBuffer_t *curr_sb;
  curr_sb = (curr_sb == sb) ? sb + 1 : sb;

  ClearOTag (curr_sb->ordering_table, OT_MAX_LEN);

  // setXY0(sprite, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  AddPrim (&curr_sb->ordering_table[1], sprite);

  DrawSync (0);
  VSync (0);

  PutDrawEnv (&curr_sb->draw_env);
  PutDispEnv (&curr_sb->disp_env);

  ClearImage (&curr_sb->draw_env.clip, 0, 0, 0);

  DrawOTag (curr_sb->ordering_table);

  FntPrint ("The quick brown fox jumps over the lazy dog.");
  FntFlush (-1);
}
