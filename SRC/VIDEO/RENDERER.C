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
#include "game/gmstate.h"
#include "game_obj/entityid.h"
#include "game_obj/pipes.h"
#include "sys/fb_ints.h"
#include "sys/fb_defs.h"
#include "video/envirnmt.h"
#include "video/scrbuff.h"
#include "video/textlkup.h"
#include "video/textmngr.h"
#include "video/uimanagr.h"

#ifdef DEBUG_BUILD
  #include <assert.h>
  #include <stdio.h>
#endif /* DEBUG_BUILD */

#define DIST_TO_SCREEN (512)

static void draw_sprites (u_long *ot, u_long *ot_idx);
static void draw_player_sprite (u_long *ot, u_long *ot_idx);
static void clear_vram (void);
static void animate_sprites (void);

void
r_init_renderer (void)
{
  //TODO: figure out how to handle NTSC/PAL differences elegantly
  SetVideoMode (0);
  GsInitGraph ((FB_SCREEN_WIDTH), (FB_SCREEN_HEIGHT), GsNONINTER|GsOFSGPU, 1,
               0);

  clear_vram (); // need to do this before `FntLoad`.
  FntLoad ((FB_SCREEN_WIDTH), 256);
  SetDumpFnt (FntOpen (0, 8, (FB_SCREEN_WIDTH), 64, 0, 512));

  SetGraphDebug (0);

  InitGeom ();  // Init GTE
  SetGeomOffset ((FB_SCREEN_WIDTH) / 2, (FB_SCREEN_HEIGHT) / 2);
  SetGeomScreen (DIST_TO_SCREEN);

  sb_init_screen_buffers ();

  tmg_auto_load_textures ();
  ev_init_background ();
  ui_init_ui_elements ();

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
  GameState_t curr_gstate = gs_get_curr_game_state ();

  curr_sb = (curr_sb == screen_buffers) ? screen_buffers + 1 : screen_buffers;
  ClearOTag (curr_sb->ordering_table, FB_ORDERING_TABLE_MAX_LENGTH);

  if ((curr_gstate != GSTATE_GAME_PAUSED) && (curr_gstate != GSTATE_GAME_OVER))
    animate_sprites ();

  ev_draw_environment (curr_sb->ordering_table, &ot_idx);

#ifdef DEBUG_BUILD
  assert(ot_idx < (FB_ORDERING_TABLE_MAX_LENGTH));
#endif /* DEBUG_BUILD */
  AddPrim(&curr_sb->ordering_table[ot_idx],
          &tpages[sprite_pools[TID_GAME_OBJ_TEXTURE].texture_id]);
  ot_idx++;

  draw_sprites (curr_sb->ordering_table, &ot_idx);
  draw_player_sprite (curr_sb->ordering_table, &ot_idx);

  ui_draw_ui_elements (curr_sb->ordering_table, &ot_idx);

  while (DrawSync (1)) {}
  VSync (0);

  PutDrawEnv (&curr_sb->draw_env);
  PutDispEnv (&curr_sb->disp_env);

//  DumpOTag (curr_sb->ordering_table);
  DrawOTag (curr_sb->ordering_table);

  FntFlush (-1);
}

void
draw_sprites (u_long *ot, u_long *ot_idx)
{
  uint8_t i;

  for (i = 1; i < sprite_pools[TID_GAME_OBJ_TEXTURE].u8_num_sprites; i++)
  {
#ifdef DEBUG_BUILD
    assert((*ot_idx) < (FB_ORDERING_TABLE_MAX_LENGTH));
#endif /* DEBUG_BUILD */
    AddPrim(&ot[(*ot_idx)],
            &sprite_pools[TID_GAME_OBJ_TEXTURE].sprites[i].sprite);
    (*ot_idx)++;
  }
}

/** Handling player separately so that they're always on top. */
void
draw_player_sprite (u_long *ot, u_long *ot_idx)
{
#ifdef DEBUG_BUILD
  assert((*ot_idx) < (FB_ORDERING_TABLE_MAX_LENGTH));
#endif /* DEBUG_BUILD */
  AddPrim (&ot[(*ot_idx)],
           &sprite_pools[TID_GAME_OBJ_TEXTURE].sprites[0].sprite);
  (*ot_idx)++;
}

void
animate_sprites (void)
{
  static uint8_t u8_num_calls = 0;
  ev_scroll_environment ();

  if ((++u8_num_calls) % 20 == 0)
  {
    sprite_pools[TID_GAME_OBJ_TEXTURE].sprites[0].sprite.u0 =
      (u8_num_calls / 20 % 4) * 20;
  }
}
