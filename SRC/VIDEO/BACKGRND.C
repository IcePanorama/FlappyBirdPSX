#include "video/backgrnd.h"
#include "video/scrbuff.h"
#include "video/textlkup.h"
#include "video/textmngr.h"
#include "sys/fb_defs.h"
#include "sys/fb_ints.h"

#ifdef DEBUG_BUILD
  #include <assert.h>
  #include <stdio.h>
#endif /* DEBUG_BUILD */

#define TEXTURE_WIDTH (255)
#define TEXTURE_HEIGHT (255)

SPRT bg_background_sprites[BGT_NUM_TILES] = {{0}};

void
bg_init_background (void)
{
  uint8_t i;
  for (i = 0; i < BGT_NUM_TILES; i++)
  {
    SetSprt(&bg_background_sprites[i]);
    SetShadeTex(&bg_background_sprites[i], 1);
    setWH(&bg_background_sprites[i], (TEXTURE_WIDTH), (TEXTURE_HEIGHT));
    setUV0(&bg_background_sprites[i], 0, 0);
  }

  for (i = 0; i < BGT_BOT_LEFT; i++)
  {
    setXY0(&bg_background_sprites[i], (TEXTURE_WIDTH) * i, 0);
    bg_background_sprites[i].clut = texture_clut_lookup[TEXTID_BG_TEXTURE];
  }

  for (i = BGT_BOT_LEFT; i < BGT_NUM_TILES; i++)
  {
    setXY0(&bg_background_sprites[i], (TEXTURE_WIDTH) * (i - BGT_BOT_LEFT), (FB_SCREEN_HEIGHT) - 30);
    bg_background_sprites[i].clut = texture_clut_lookup[TEXTID_FG_TEXTURE];
  }
}

void
bg_draw_background (u_long *ot, u_long *ot_idx)
{
  uint8_t i;

#ifdef DEBUG_BUILD
  assert((*ot_idx) < (OT_MAX_LEN));
#endif /* DEBUG_BUILD */
  AddPrim(&ot[(*ot_idx)], &tpages[TEXTID_BG_TEXTURE]);
  (*ot_idx)++;

  for (i = 0; i < BGT_BOT_LEFT; i++)
  {
#ifdef DEBUG_BUILD
    assert((*ot_idx) < (OT_MAX_LEN));
#endif /* DEBUG_BUILD */
    AddPrim(&ot[(*ot_idx)], &bg_background_sprites[i]);
    (*ot_idx)++;
  }
}

void
bg_draw_foreground (u_long *ot, u_long *ot_idx)
{
  uint8_t i;

#ifdef DEBUG_BUILD
  assert((*ot_idx) < (OT_MAX_LEN));
#endif /* DEBUG_BUILD */
  AddPrim(&ot[(*ot_idx)], &tpages[TEXTID_FG_TEXTURE]);
  (*ot_idx)++;

  for (i = BGT_BOT_LEFT; i < BGT_NUM_TILES; i++)
  {
#ifdef DEBUG_BUILD
    assert((*ot_idx) < (OT_MAX_LEN));
#endif /* DEBUG_BUILD */
    AddPrim(&ot[(*ot_idx)], &bg_background_sprites[i]);
    (*ot_idx)++;
  }
}

void
bg_scroll_background (void)
{

  uint8_t i;
  for (i = 0; i < BGT_BOT_LEFT; i++)
  {
    bg_background_sprites[i].u0++;
  }

  for (i = BGT_BOT_LEFT; i < BGT_NUM_TILES; i++)
  {
    bg_background_sprites[i].u0 += 2;
  }
}
