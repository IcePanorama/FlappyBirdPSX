#include "video/uimanagr.h"
#include "game/gmstate.h"
#include "sys/fb_bools.h"
#include "sys/fb_defs.h"
#include "video/textid.h"
#include "video/textlkup.h"
#include "video/textmngr.h"

/* Do not touch! CC seems really particular about the order of these. */
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libetc.h>
/**********************************************************************/

#ifdef DEBUG_BUILD
  #include "video/vid_defs.h"

  #include <assert.h>
  #include <stdio.h>
#endif /* DEBUG_BUILD */

#define UI_START_Y (32)

/* 'Game Over' Text Defines */
#define GAME_OVER_TEXT_WIDTH (96)
#define GAME_OVER_TEXT_HEIGHT (32)
#define GAME_OVER_TEXT_X_POS \
  (((FB_SCREEN_WIDTH) >> 1) - ((GAME_OVER_TEXT_WIDTH) >> 1))
#define GAME_OVER_TEXT_Y_POS ((UI_START_Y))
#define GAME_OVER_TEXT_V0 (64)
/****************************/

/* Game Over Score Box Defines */
#define GAME_OVER_SCORE_BOX_WIDTH (128)
#define GAME_OVER_SCORE_BOX_HEIGHT (64)
#define GAME_OVER_SCORE_BOX_X_POS \
  (((FB_SCREEN_WIDTH) >> 1) - ((GAME_OVER_SCORE_BOX_WIDTH) >> 1))
#define GAME_OVER_SCORE_BOX_Y_POS \
  (((FB_SCREEN_HEIGHT) >> 1) - ((GAME_OVER_SCORE_BOX_HEIGHT) >> 1))
#define GAME_OVER_SCORE_BOX_V0 (0)
/*******************************/

/* Gold and Silver Medal Defines */
#define MEDAL_SIZE (32)
#define MEDAL_WIDTH ((MEDAL_SIZE))
#define MEDAL_HEIGHT ((MEDAL_SIZE))
#define MEDAL_X_POS ((GAME_OVER_SCORE_BOX_X_POS) + 10)
#define MEDAL_Y_POS (((FB_SCREEN_HEIGHT) >> 1) - ((MEDAL_HEIGHT) >> 1))
#define SILVER_MEDAL_V0 (96)
#define GOLD_MEDAL_V0 (128)
/*********************************/

/* Score Text Defines */
#define SCORE_MAX_NUM_DIGITS (3)
#define SCORE_TEXT_WIDTH (16)
#define SCORE_TEXT_HEIGHT (32)
#define SCORE_TEXT_NORMAL_X_POS \
  (((FB_SCREEN_WIDTH) >> 1) - (SCORE_TEXT_WIDTH >> 1))
#define SCORE_TEXT_NORMAL_Y_POS ((UI_START_Y))
#define SCORE_TEXT_GAME_OVER_X_POS \
  ((GAME_OVER_SCORE_BOX_X_POS) + ((GAME_OVER_SCORE_BOX_WIDTH) - 6) - (SCORE_TEXT_WIDTH))
#define SCORE_TEXT_GAME_OVER_Y_POS ((GAME_OVER_SCORE_BOX_Y_POS) - 1)
#define SCORE_BIG_TEXT_V0 (0)
/**********************/

static SPRT s_game_over_text = {0};
static SPRT s_game_over_score_box = {0};
static SPRT s_silver_medal = {0};
static SPRT s_gold_medal = {0};
static UIGameOverCoinState_t game_over_coin_state = UICS_NO_COIN;

static uint16_t *curr_score_ptr = 0;
static uint16_t *high_score_ptr = 0;
static SPRT s_score_normal_text[SCORE_MAX_NUM_DIGITS] = {{0}};
static SPRT s_score_game_over_text[SCORE_MAX_NUM_DIGITS] = {{0}};
static uint8_t u8_num_digits = 1;

static void init_ui_element (SPRT* sptr_element, const uint16_t u16_width,
                             const uint16_t u16_height, const uint16_t u16_x0,
                             const uint16_t u16_y0, const uint16_t u16_v0);
static void init_score_text_element (SPRT* sptr_element);
static void draw_ui_game_over (u_long *ot, u_long *ot_idx);
static void draw_ui_normal (u_long *ot, u_long *ot_idx);
static void draw_ui_score_text (u_long *ot, u_long *ot_idx,
                                SPRT *sptr_score_text);
static void draw_ui_element (u_long *ot, u_long *ot_idx, SPRT* sptr_element);

void
ui_init_ui_elements (void)
{
  uint8_t i;

  init_ui_element (&s_game_over_text, (GAME_OVER_TEXT_WIDTH),
                   (GAME_OVER_TEXT_HEIGHT), (GAME_OVER_TEXT_X_POS),
                   (GAME_OVER_TEXT_Y_POS), (GAME_OVER_TEXT_V0));
  init_ui_element (&s_game_over_score_box, (GAME_OVER_SCORE_BOX_WIDTH),
                   (GAME_OVER_SCORE_BOX_HEIGHT), (GAME_OVER_SCORE_BOX_X_POS),
                   (GAME_OVER_SCORE_BOX_Y_POS), (GAME_OVER_SCORE_BOX_V0));
  init_ui_element (&s_silver_medal, (MEDAL_WIDTH), (MEDAL_HEIGHT),
                   (MEDAL_X_POS), (MEDAL_Y_POS), (SILVER_MEDAL_V0));
  init_ui_element (&s_gold_medal, (MEDAL_WIDTH), (MEDAL_HEIGHT), (MEDAL_X_POS),
                   (MEDAL_Y_POS), (GOLD_MEDAL_V0));

  memset (s_score_normal_text, 0, sizeof (SPRT) * (SCORE_MAX_NUM_DIGITS));
  memset (s_score_game_over_text, 0, sizeof (SPRT) * (SCORE_MAX_NUM_DIGITS));
  for (i = 0; i < (SCORE_MAX_NUM_DIGITS); i++)
  {
    init_score_text_element (&s_score_normal_text[i]);
    init_score_text_element (&s_score_game_over_text[i]);
    setXY0(&s_score_normal_text[i], (SCORE_TEXT_NORMAL_X_POS),
           (SCORE_TEXT_NORMAL_Y_POS));
    setXY0(&s_score_game_over_text[i], (SCORE_TEXT_GAME_OVER_X_POS),
           (SCORE_TEXT_GAME_OVER_Y_POS));
  }

  u8_num_digits = 1;
}

void
init_ui_element (SPRT* sptr_element, const uint16_t u16_width,
                 const uint16_t u16_height, const uint16_t u16_x0,
                 const uint16_t u16_y0, const uint16_t u16_v0)
{
  SetSprt(sptr_element);
  SetShadeTex(sptr_element, 1);
  setWH(sptr_element, u16_width, u16_height);
  setUV0(sptr_element, 0, u16_v0);
  setXY0(sptr_element, u16_x0, u16_y0);
  sptr_element->clut = texture_clut_lookup[TEXTID_UI_TEXTURE];
}

void
init_score_text_element (SPRT* sptr_element)
{
  SetSprt(sptr_element);
  SetShadeTex(sptr_element, 1);
  setWH(sptr_element, (SCORE_TEXT_WIDTH), (SCORE_TEXT_HEIGHT));
  setUV0(sptr_element, 0, SCORE_BIG_TEXT_V0);
  sptr_element->clut = texture_clut_lookup[TEXTID_FONT_TEXTURE];
}

void
ui_init_score_text (uint16_t *u16ptr_current_score, uint16_t *u16ptr_high_score)
{
#ifdef DEBUG_BUILD
  assert(u16ptr_current_score != 0);
  assert(u16ptr_high_score != 0);
#endif /* DEBUG_BUILD */

  curr_score_ptr = u16ptr_current_score;
  high_score_ptr = u16ptr_high_score;
}

void
ui_draw_ui_elements (u_long *ot, u_long *ot_idx)
{
  switch (gs_get_curr_game_state ())
  {
    case GSTATE_GAME_START:
    case GSTATE_NORMAL:
      draw_ui_normal (ot, ot_idx);
      break;
    case GSTATE_GAME_OVER:
      draw_ui_game_over (ot, ot_idx);
      break;
    default:
      break;
  }
}

void
draw_ui_normal (u_long *ot, u_long *ot_idx)
{
  draw_ui_score_text (ot, ot_idx, s_score_normal_text);
}

void
draw_ui_score_text (u_long *ot, u_long *ot_idx, SPRT *sptr_score_text)
{
  uint8_t i;

#ifdef DEBUG_BUILD
  assert((*ot_idx) < (FB_ORDERING_TABLE_MAX_LENGTH));
#endif /* DEBUG_BUILD */
  AddPrim(&ot[(*ot_idx)], &tpages[TEXTID_FONT_TEXTURE]);
  (*ot_idx)++;

  for (i = 0; i < u8_num_digits; i++)
  {
#ifdef DEBUG_BUILD
    assert((*ot_idx) < (FB_ORDERING_TABLE_MAX_LENGTH));
#endif /* DEBUG_BUILD */
    AddPrim (&ot[(*ot_idx)], &sptr_score_text[i]);
    (*ot_idx)++;
  }
}

void
draw_ui_game_over (u_long *ot, u_long *ot_idx)
{
#ifdef DEBUG_BUILD
  assert((*ot_idx) < (FB_ORDERING_TABLE_MAX_LENGTH));
#endif /* DEBUG_BUILD */
  AddPrim(&ot[(*ot_idx)], &tpages[TEXTID_UI_TEXTURE]);
  (*ot_idx)++;

  draw_ui_element(ot, ot_idx, &s_game_over_text);
  draw_ui_element(ot, ot_idx, &s_game_over_score_box);
  draw_ui_score_text (ot, ot_idx, s_score_game_over_text);

  switch (game_over_coin_state)
  {
    case UICS_SILVER_COIN:
      draw_ui_element(ot, ot_idx, &s_silver_medal);
      break;
    case UICS_GOLD_COIN:
      draw_ui_element(ot, ot_idx, &s_gold_medal);
      break;
    case UICS_NO_COIN:
    default:
      break;
  }
}

void
draw_ui_element (u_long *ot, u_long *ot_idx, SPRT* sptr_element)
{
#ifdef DEBUG_BUILD
  assert((*ot_idx) < (FB_ORDERING_TABLE_MAX_LENGTH));
#endif /* DEBUG_BUILD */
  AddPrim (&ot[(*ot_idx)], sptr_element);
  (*ot_idx)++;
}

void
ui_set_game_over_coin_state (UIGameOverCoinState_t state)
{
#ifdef DEBUG_BUILD
  assert(state < UICS_NUM_COIN_STATES);
#endif /* DEBUG_BUILD */
  game_over_coin_state = state;
}

//TODO: implement these two
void
ui_update_current_score_text (void)
{

}

void
ui_update_high_score_text (void)
{
}
