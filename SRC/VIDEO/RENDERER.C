/* Do not touch! CC seems really particular about the order of these. */
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libetc.h>
/******************/

#include "video/renderer.h"
#include "compnts/sprites.h"
#include "compnts/wiframe.h"
#include "game/gamemngr.h"
#include "game/scrwritr.h"
#include "game_obj/entityid.h"
#include "game_obj/pipes.h"
#include "sys/fb_ints.h"
#include "sys/fb_defs.h"
#include "video/backgrnd.h"
#include "video/scrbuff.h"
#include "video/textlkup.h"
#include "video/textmngr.h"

#ifdef DEBUG_BUILD
  #include <assert.h>
  #include <stdio.h>
#endif /* DEBUG_BUILD */

//tmp
#include "sys/fb_bools.h"
#include <libcd.h>

#define DIST_TO_SCREEN (512)

static void draw_sprites (u_long *ot, u_long *ot_idx);
static void draw_player_sprite (u_long *ot, u_long *ot_idx);
static void dump_font_img (u_long *ot, u_long *ot_idx);
static void draw_text_output (u_long *ot, u_long *ot_idx);
static void clear_vram (void);

void
r_init_renderer (void)
{
  //TODO: figure out how to handle NTSC/PAL differences elegantly
  SetVideoMode (0);
  GsInitGraph ((FB_SCREEN_WIDTH), (FB_SCREEN_HEIGHT), GsNONINTER|GsOFSGPU, 1, 0);

  FntLoad (960, 256);
  SetDumpFnt (FntOpen (0, 8, (FB_SCREEN_WIDTH), 64, 0, 512));

  SetGraphDebug (0);

  InitGeom ();  // Init GTE
  SetGeomOffset ((FB_SCREEN_WIDTH) / 2, (FB_SCREEN_HEIGHT) / 2);
  SetGeomScreen (DIST_TO_SCREEN);

  sb_init_screen_buffers ();

  clear_vram ();

  tmg_auto_load_textures ();
  SetDrawTPage(&player_tpage, 0, 0,
               texture_tpage_lookup[TEXTID_PIPES_TEXTURE]);
  bg_init_background();
  sw_init ();

  SetDispMask (1);
}

/**
 *  Not strictly necessary, but this should prevent any odd bugs from whatever
 *  random garbage is there in the future.
 */
void
clear_vram (void)
{
  RECT r;
  r.x = r.y = 0;
  r.w = 1024;
  r.h = 512;
  ClearImage(&r, 0, 0, 0);
}

void
r_render_screen (void)
{
  u_long ot_idx = 0;
  static ScreenBuffer_t *curr_sb = screen_buffers;
  curr_sb = (curr_sb == screen_buffers) ? screen_buffers + 1 : screen_buffers;

  ClearOTag (curr_sb->ordering_table, OT_MAX_LEN);

  bg_draw_background (curr_sb->ordering_table, &ot_idx);

  draw_sprites (curr_sb->ordering_table, &ot_idx);
  bg_draw_foreground (curr_sb->ordering_table, &ot_idx);

  /* Handling player separately so that they're always on top. */
  draw_player_sprite (curr_sb->ordering_table, &ot_idx);

  //FIXME: make this a macro function to get rid of the compiler warning.
  if (FALSE)  dump_font_img (curr_sb->ordering_table, &ot_idx);

  draw_text_output (curr_sb->ordering_table, &ot_idx);

  DrawSync (0);
  VSync (0);

  PutDrawEnv (&curr_sb->draw_env);
  PutDispEnv (&curr_sb->disp_env);

  ClearImage (&curr_sb->draw_env.clip, 0, 0, 0);

//  DumpOTag (curr_sb->ordering_table);
  DrawOTag (curr_sb->ordering_table);

  FntFlush (-1);
}

void
draw_sprites (u_long *ot, u_long *ot_idx)
{
  uint8_t i;
  uint8_t j;

  for (i = 0; i < TEXTID_NUM_TEXTURES; i++)
  {
    /* Skip unused sprites. */
    if (sprite_pools[i].u8_num_sprites == 0) continue;

#ifdef DEBUG_BUILD
    assert((*ot_idx) < (OT_MAX_LEN));
#endif /* DEBUG_BUILD */
    AddPrim(&ot[(*ot_idx)], &tpages[sprite_pools[i].texture_id]);
    (*ot_idx)++;

    for (j = 0; j < sprite_pools[i].u8_num_sprites; j++)
    {
      /* Player sprite is handled separately. */
      if (sprite_pools[i].sprites[j].u8_parent_id == EID_PLAYER_ID)
        continue;

#ifdef DEBUG_BUILD
      assert((*ot_idx) < (OT_MAX_LEN));
#endif /* DEBUG_BUILD */
      AddPrim(&ot[(*ot_idx)], &sprite_pools[sprite_pools[i].texture_id].sprites[j].sprite);
      (*ot_idx)++;
    }
  }
}

void
draw_player_sprite (u_long *ot, u_long *ot_idx)
{
  /* Draw player sprite on top. This assumes player is always sprite 0. */
#ifdef DEBUG_BUILD
  assert((*ot_idx) < (OT_MAX_LEN));
#endif /* DEBUG_BUILD */
  AddPrim(&ot[(*ot_idx)], &player_tpage);
  (*ot_idx)++;

#ifdef DEBUG_BUILD
  assert((*ot_idx) < (OT_MAX_LEN));
#endif /* DEBUG_BUILD */
  AddPrim (&ot[(*ot_idx)],
           &sprite_pools[(TEXTID_PIPES_TEXTURE)].sprites[0].sprite);
  (*ot_idx)++;
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
                255, 0,
                                          0, 63,
                255, 63);

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
