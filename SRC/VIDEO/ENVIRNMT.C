#include "video/envirnmt.h"
#include "video/textlkup.h"
#include "video/textmngr.h"
#include "sys/fb_defs.h"
#include "sys/fb_ints.h"

#ifdef DEBUG_BUILD
  #include "video/vid_defs.h"

  #include <assert.h>
  #include <stdio.h>
#endif /* DEBUG_BUILD */

#define TEXTURE_WIDTH (255)
#define TEXTURE_HEIGHT (255)

#define FOREGROUND_HEIGHT (FB_FOREGROUND_HEIGHT)

enum EVTiles_e
{
 EVT_TOP_LEFT,
 EVT_TOP_RIGHT,
 EVT_BOT_LEFT,
 EVT_BOT_RIGHT,
 EVT_NUM_TILES
};

static SPRT ev_background_sprites[EVT_NUM_TILES] = {{0}};

void
ev_init_background (void)
{
  uint8_t i;
  for (i = 0; i < EVT_NUM_TILES; i++)
  {
    SetSprt(&ev_background_sprites[i]);
    SetShadeTex(&ev_background_sprites[i], 1);
    setUV0(&ev_background_sprites[i], 0, 0);
  }

  setWH(&ev_background_sprites[0], (TEXTURE_WIDTH), (FB_SCREEN_HEIGHT) - (FOREGROUND_HEIGHT));
  setWH(&ev_background_sprites[1], (FB_SCREEN_WIDTH) - (TEXTURE_WIDTH), (FB_SCREEN_HEIGHT) - (FOREGROUND_HEIGHT));
  for (i = 0; i < EVT_BOT_LEFT; i++)
  {
    setXY0(&ev_background_sprites[i], (TEXTURE_WIDTH) * i, 0);
    ev_background_sprites[i].clut = texture_clut_lookup[TEXTID_BG_TEXTURE];
  }

  setWH(&ev_background_sprites[2], (TEXTURE_WIDTH), (FOREGROUND_HEIGHT));
  setWH(&ev_background_sprites[3], (FB_SCREEN_WIDTH) - (TEXTURE_WIDTH), (FOREGROUND_HEIGHT));
  for (i = EVT_BOT_LEFT; i < EVT_NUM_TILES; i++)
  {
    setXY0(&ev_background_sprites[i], (TEXTURE_WIDTH) * (i - EVT_BOT_LEFT), (FB_SCREEN_HEIGHT) - (FOREGROUND_HEIGHT));
    ev_background_sprites[i].clut = texture_clut_lookup[TEXTID_FG_TEXTURE];
  }
}

void
ev_draw_background (u_long *ot, u_long *ot_idx)
{
  uint8_t i;

#ifdef DEBUG_BUILD
  assert((*ot_idx) < (FB_ORDERING_TABLE_MAX_LENGTH));
#endif /* DEBUG_BUILD */
  AddPrim(&ot[(*ot_idx)], &tpages[TEXTID_BG_TEXTURE]);
  (*ot_idx)++;

  for (i = 0; i < EVT_BOT_LEFT; i++)
  {
#ifdef DEBUG_BUILD
    assert((*ot_idx) < (FB_ORDERING_TABLE_MAX_LENGTH));
#endif /* DEBUG_BUILD */
    AddPrim(&ot[(*ot_idx)], &ev_background_sprites[i]);
    (*ot_idx)++;
  }
}

void
ev_draw_foreground (u_long *ot, u_long *ot_idx)
{
  uint8_t i;

#ifdef DEBUG_BUILD
  assert((*ot_idx) < (FB_ORDERING_TABLE_MAX_LENGTH));
#endif /* DEBUG_BUILD */
  AddPrim(&ot[(*ot_idx)], &tpages[TEXTID_FG_TEXTURE]);
  (*ot_idx)++;

  for (i = EVT_BOT_LEFT; i < EVT_NUM_TILES; i++)
  {
#ifdef DEBUG_BUILD
    assert((*ot_idx) < (FB_ORDERING_TABLE_MAX_LENGTH));
#endif /* DEBUG_BUILD */
    AddPrim(&ot[(*ot_idx)], &ev_background_sprites[i]);
    (*ot_idx)++;
  }
}

void
ev_scroll_background (void)
{

  uint8_t i;
  for (i = 0; i < EVT_BOT_LEFT; i++)
    ev_background_sprites[i].u0++;

  for (i = EVT_BOT_LEFT; i < EVT_NUM_TILES; i++)
    ev_background_sprites[i].u0 += 2;
}

void
ev_reset (void)
{

  uint8_t i;
  for (i = 0; i < EVT_NUM_TILES; i++)
    ev_background_sprites[i].u0 = 0;
}
