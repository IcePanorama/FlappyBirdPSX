/* Do not touch! CC seems really particular about the order of these. */
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libetc.h>
/******************/

#include "compnts/sprites.h"
#include "compnts/wiframe.h"
#include "sys/fb_ints.h"
#include "sys/fb_defs.h"
#include "video/scrbuff.h"
#include "video/video.h"

#ifdef DEBUG_BUILD
#include <assert.h>
#include <stdio.h>
#endif /* DEBUG_BUILD */

#define DIST_TO_SCREEN (512)

static void draw_sprites (u_long *ot, u_long *ot_idx);
static void draw_wireframes (u_long *ot, u_long *ot_idx);

void
v_init_video (void)
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

  sb_init_screen_buffers ();

  SetDispMask (1);
}

void
v_render_screen (void)
{
  u_long ot_idx = 0;
  static ScreenBuffer_t *curr_sb = screen_buffers;
  curr_sb = (curr_sb == screen_buffers) ? screen_buffers + 1 : screen_buffers;

  ClearOTag (curr_sb->ordering_table, OT_MAX_LEN);

  draw_sprites (curr_sb->ordering_table, &ot_idx);

  /* Draw player sprite on top. This assumes player is always sprite 0. */
  AddPrim (&curr_sb->ordering_table[ot_idx], &sp_sprite_pool[0].p4_sprite);
  ot_idx++;

  draw_wireframes (curr_sb->ordering_table, &ot_idx);

  DrawSync (0);
  VSync (0);

  PutDrawEnv (&curr_sb->draw_env);
  PutDispEnv (&curr_sb->disp_env);

  ClearImage (&curr_sb->draw_env.clip, 100, 100, 100);

  DumpOTag (curr_sb->ordering_table);
  DrawOTag (curr_sb->ordering_table);

  FntFlush (-1);
}

void
draw_sprites (u_long *ot, u_long *ot_idx)
{
  uint8_t i;

  for (i = 1; i < sp_num_sprites; i++)
  {
#ifdef DEBUG_BUILD
    assert((*ot_idx) < OT_MAX_LEN);
#endif /* DEBUG_BUILD */

    AddPrim (&ot[(*ot_idx)], &sp_sprite_pool[i].p4_sprite);
    (*ot_idx)++;
  }
}

void
draw_wireframes (u_long *ot, u_long *ot_idx)
{
  uint8_t i;
  uint8_t j;

  for (i = 0; i < u8_wfc_num_wireframes; i++)
  {
    for (j = 0; j < WIFRAME_NUM_WIRES; j++)
    {
#ifdef DEBUG_BUILD
      assert((*ot_idx) < OT_MAX_LEN);
#endif /* DEBUG_BUILD */

      AddPrim (&ot[(*ot_idx)], &wfc_wireframe_pool[i].wires[j]);
      (*ot_idx)++;
    }
  }
}

#undef DIST_TO_SCREEN
#undef OT_MAX_LEN
