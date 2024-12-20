/* Do not touch! CC seems really particular about the order of these. */
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libetc.h>
/******************/

#include "video/video.h"
#include "compnts/sprites.h"
#include "compnts/wiframe.h"
#include "game/gamemngr.h"
#include "game/scrwritr.h"
#include "game_obj/pipes.h"
#include "sys/fb_ints.h"
#include "sys/fb_defs.h"
#include "video/scrbuff.h"
#include "video/textlkup.h"

#ifdef DEBUG_BUILD
  #include <assert.h>
  #include <stdio.h>
#endif /* DEBUG_BUILD */

//tmp
#include "sys/fb_bools.h"
#include <libcd.h>

#define DIST_TO_SCREEN (512)

static void draw_sprites (u_long *ot, u_long *ot_idx);
static void dump_font_img (u_long *ot, u_long *ot_idx);
static void draw_text_output (u_long *ot, u_long *ot_idx);
static void load_textures (void);

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

  sw_init ();
  load_textures ();

	SetDispMask (1);
}

/** TODO: dynamically load/position textures into vram */
void
load_textures (void)
{
  CdlFILE fptr;
  u_long sprite_data[((2) << 11)] = {0};
  TIM_IMAGE sprite_img;

  assert(CdSearchFile (&fptr, "\\ASSETS\\SPRITES\\PIPE.TIM;1") != 0);
  assert(CdControlB (CdlSetloc, (u_char *)&fptr.pos, 0) != 0);
  assert(CdRead ((2), sprite_data, CdlModeSpeed) != 0);
  CdReadSync(0, 0); // wait for operation to finish.

  assert(OpenTIM (sprite_data) == 0);
  assert(ReadTIM (&sprite_img) != 0);
  LoadImage (sprite_img.crect, sprite_img.caddr);
  DrawSync (0);
  LoadImage (sprite_img.prect, sprite_img.paddr);
  DrawSync (0);

  assert(CdSearchFile (&fptr, "\\ASSETS\\SPRITES\\TOP_PIPE.TIM;1") != 0);
  assert(CdControlB (CdlSetloc, (u_char *)&fptr.pos, 0) != 0);
  assert(CdRead ((2), sprite_data, CdlModeSpeed) != 0);
  CdReadSync(0, 0); // wait for operation to finish.

  assert(OpenTIM (sprite_data) == 0);
  assert(ReadTIM (&sprite_img) != 0);
  sprite_img.crect->x = 0;
  sprite_img.crect->y = 482;
  LoadImage (sprite_img.crect, sprite_img.caddr);
  DrawSync (0);
  sprite_img.prect->x = 320+64;
  sprite_img.prect->y = 256;
  LoadImage (sprite_img.prect, sprite_img.paddr);
  DrawSync (0);

  texture_tpage_lookup[TEXTID_PIPE_BOT_TEXTURE] = GetTPage (0, 0, 320, 256);
  texture_clut_lookup[TEXTID_PIPE_BOT_TEXTURE] = GetClut (0, 481);
  texture_tpage_lookup[TEXTID_TMP] = GetTPage (0, 0, 320+64, 256);
  texture_clut_lookup[TEXTID_TMP] = GetClut (0, 482);

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
#ifdef DEBUG_BUILD
  assert(ot_idx < (OT_MAX_LEN));
#endif /* DEBUG_BUILD */
  AddPrim (&curr_sb->ordering_table[ot_idx], &sp_sprite_pool[0].p4_sprite);
  ot_idx++;

  if (FALSE) dump_font_img (curr_sb->ordering_table, &ot_idx);

  draw_text_output (curr_sb->ordering_table, &ot_idx);

  DrawSync (0);
  VSync (0);

  PutDrawEnv (&curr_sb->draw_env);
  PutDispEnv (&curr_sb->disp_env);

  ClearImage (&curr_sb->draw_env.clip, 100, 100, 100);

//  DumpOTag (curr_sb->ordering_table);
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
    assert((*ot_idx) < (OT_MAX_LEN));
#endif /* DEBUG_BUILD */

    AddPrim (&ot[(*ot_idx)], &sp_sprite_pool[i].p4_sprite);
    (*ot_idx)++;
  }
}

void
dump_font_img (u_long *ot, u_long *ot_idx)
{
  static POLY_FT4 test;

  SetPolyFT4 (&test);
  SetShadeTex (&test, 1);
  test.tpage = texture_tpage_lookup[TEXTID_FONT_TEXTURE];
  test.clut = texture_clut_lookup[TEXTID_FONT_TEXTURE];
  setXY4(&test,                           10,                            50,
                (SW_FONT_TEXTURE_WIDTH) + 10,                            50,
                                          10, (SW_FONT_TEXTURE_HEIGHT) + 50,
                (SW_FONT_TEXTURE_WIDTH) + 10, (SW_FONT_TEXTURE_HEIGHT) + 50
  );
  setUV4(&test,                           0, 0,
                (SW_FONT_TEXTURE_WIDTH) - 1, 0,
                                          0, (SW_FONT_TEXTURE_HEIGHT) - 1,
                (SW_FONT_TEXTURE_WIDTH) - 1, (SW_FONT_TEXTURE_HEIGHT) - 1);

#ifdef DEBUG_BUILD
  assert((*ot_idx) < (OT_MAX_LEN));
#endif /* DEBUG_BUILD */
  AddPrim (&ot[(*ot_idx)], &test);
  (*ot_idx)++;
}

void
draw_text_output (u_long *ot, u_long *ot_idx)
{
  uint8_t i;
  uint8_t j;
  static DR_TPAGE tpage;


  SetDrawTPage(&tpage, 0, 0, texture_tpage_lookup[TEXTID_FONT_TEXTURE]);
#ifdef DEBUG_BUILD
  assert((*ot_idx) < (OT_MAX_LEN));
#endif /* DEBUG_BUILD */
  AddPrim(&ot[(*ot_idx)], &tpage);
  (*ot_idx)++;

  for (i = 0; i < sw_num_outputs; i++)
  {
    for (j = 0; j < sw_output_pool[i].u8_length; j++)
    {
      if (sw_output_pool[i].zcs_msg[j] == '\0') break;
#ifdef DEBUG_BUILD
      assert((*ot_idx) < (OT_MAX_LEN));
#endif /* DEBUG_BUILD */

      AddPrim (&ot[(*ot_idx)], &sw_output_pool[i].glyphs[j]);
      (*ot_idx)++;
    }
  }
}
