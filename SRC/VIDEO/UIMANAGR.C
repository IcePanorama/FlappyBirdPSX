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
#define GAME_OVER_GRAPHIC_WIDTH  (96)
#define GAME_OVER_GRAPHIC_HEIGHT (32)
#define GAME_OVER_GRAPHIC_X_POS  \
  (((FB_SCREEN_WIDTH) >> 1) - ((GAME_OVER_GRAPHIC_WIDTH) >> 1))
#define GAME_OVER_GRAPHIC_Y_POS  ((UI_START_Y))
#define GAME_OVER_GRAPHIC_V0     (64)
/****************************/

/* 'Get Ready!' Graphic Defines */
#define GET_READY_GRAPHIC_V0     (128)
#define GET_READY_GRAPHIC_Y_POS  ((GAME_OVER_GRAPHIC_Y_POS) + 5)
/********************************/

/* Game Over Score Box Defines */
#define GAME_OVER_SCORE_BOX_WIDTH  (128)
#define GAME_OVER_SCORE_BOX_HEIGHT (64)
#define GAME_OVER_SCORE_BOX_X_POS  \
  (((FB_SCREEN_WIDTH) >> 1) - ((GAME_OVER_SCORE_BOX_WIDTH) >> 1))
#define GAME_OVER_SCORE_BOX_Y_POS  \
  (((FB_SCREEN_HEIGHT) >> 1) - ((GAME_OVER_SCORE_BOX_HEIGHT) >> 1))
#define GAME_OVER_SCORE_BOX_V0     (0)
/*******************************/

/* Gold and Silver Medal Defines */
#define MEDAL_SIZE    (32)
#define MEDAL_WIDTH   ((MEDAL_SIZE))
#define MEDAL_HEIGHT  ((MEDAL_SIZE))
#define MEDAL_X_POS   ((GAME_OVER_SCORE_BOX_X_POS) + 10)
#define MEDAL_Y_POS   (((FB_SCREEN_HEIGHT) >> 1) - ((MEDAL_HEIGHT) >> 1))
#define MEDALS_V0     (96)
#define GOLD_MEDAL_U0 (32)
/*********************************/

/* Tutorial Graphic Defines */
#define TUTO_GRAPHIC_WIDTH  (64)
#define TUTO_GRAPHIC_HEIGHT (32)
#define TUTO_GRAPHIC_X_POS  (((FB_SCREEN_WIDTH) >> 1) - ((TUTO_GRAPHIC_WIDTH) >> 1))
#define TUTO_GRAPHIC_Y_POS  ((GAME_OVER_SCORE_BOX_Y_POS) + (GAME_OVER_SCORE_BOX_HEIGHT) + ((TUTO_GRAPHIC_HEIGHT) >> 1))
#define TUTO_GRAPHIC_V0     (160)
/****************************/

/* Score Text Defines */
#define SCORE_MAX_NUM_DIGITS (3)

// Normal game mode text.
#define SCORE_TEXT_WIDTH      (16)
#define SCORE_TEXT_HALF_WIDTH ((SCORE_TEXT_WIDTH) >> 1)
#define SCORE_TEXT_HEIGHT     (32)
#define SCORE_TEXT_X_POS      \
  (((FB_SCREEN_WIDTH) >> 1) - (SCORE_TEXT_WIDTH >> 1))
#define SCORE_TEXT_Y_POS      ((UI_START_Y))
#define SCORE_TEXT_V0         (0)

// Game over text.
#define SCORE_TEXT_GAME_OVER_WIDTH      (12)
#define SCORE_TEXT_GAME_OVER_HALF_WIDTH ((SCORE_TEXT_GAME_OVER_WIDTH) >> 1)
#define SCORE_TEXT_GAME_OVER_HEIGHT     (16)
#define SCORE_TEXT_GAME_OVER_X_POS      \
  ((GAME_OVER_SCORE_BOX_X_POS) + (GAME_OVER_SCORE_BOX_WIDTH) - (SCORE_TEXT_GAME_OVER_WIDTH) - 5)
#define SCORE_TEXT_GAME_OVER_Y_OFFSET   (14)
#define SCORE_TEXT_GAME_OVER_Y_POS      \
  ((GAME_OVER_SCORE_BOX_Y_POS) + (SCORE_TEXT_GAME_OVER_Y_OFFSET))
#define SCORE_TEXT_GAME_OVER_V0         (32)

// High score text.
#define HIGH_SCORE_TEXT_X_POS    (SCORE_TEXT_GAME_OVER_X_POS)
#define HIGH_SCORE_TEXT_Y_OFFSET (39)
#define HIGH_SCORE_TEXT_Y_POS    \
  ((GAME_OVER_SCORE_BOX_Y_POS) + (HIGH_SCORE_TEXT_Y_OFFSET))
/**********************/

static UIGameOverCoinState_t game_over_coin_state = UICS_NO_COIN;

static SPRT s_game_over_graphic   = {0};
static SPRT s_game_over_score_box = {0};
static SPRT s_silver_medal        = {0};
static SPRT s_gold_medal          = {0};
static SPRT s_get_ready_graphic   = {0};
static SPRT s_tutorial_graphic   = {0};

static SPRT s_score_normal_text[(SCORE_MAX_NUM_DIGITS)]    = {{0}};
static SPRT s_score_game_over_text[(SCORE_MAX_NUM_DIGITS)] = {{0}};
static SPRT s_high_score_text[(SCORE_MAX_NUM_DIGITS)]      = {{0}};

static POLY_F4 pf4_pause_overlay = {0};

static uint8_t u8_num_digits            = 1;
static uint8_t u8_high_score_num_digits = 1;

static void init_ui_element (SPRT* sptr_element, const uint16_t u16_width,
                             const uint16_t u16_height, const uint16_t u16_x0,
                             const uint16_t u16_y0, const uint16_t u16_v0);
static void init_score_text_element (SPRT* sptr_element);
static void draw_ui_game_over (u_long *ot, u_long *ot_idx);
static void draw_ui_game_start (u_long *ot, u_long *ot_idx);
static void draw_ui_normal (u_long *ot, u_long *ot_idx);
static void draw_ui_game_paused (u_long *ot, u_long *ot_idx);
static void draw_ui_score_text (u_long *ot, u_long *ot_idx,
                                SPRT *sptr_score_text);
static void draw_ui_high_score_text (u_long *ot, u_long *ot_idx,
                                     SPRT *sptr_score_text);
static void draw_ui_element (u_long *ot, u_long *ot_idx, SPRT* sptr_element);
static void add_font_tpage_to_ot (u_long *ot, u_long *ot_idx);
static uint8_t calc_num_digits (uint16_t x);

void
ui_init_ui_elements (void)
{
  uint8_t i;

  init_ui_element (&s_game_over_graphic, (GAME_OVER_GRAPHIC_WIDTH),
                   (GAME_OVER_GRAPHIC_HEIGHT), (GAME_OVER_GRAPHIC_X_POS),
                   (GAME_OVER_GRAPHIC_Y_POS), (GAME_OVER_GRAPHIC_V0));
  init_ui_element (&s_get_ready_graphic, (GAME_OVER_GRAPHIC_WIDTH),
                   (GAME_OVER_GRAPHIC_HEIGHT), (GAME_OVER_GRAPHIC_X_POS),
                   (GET_READY_GRAPHIC_Y_POS), (GET_READY_GRAPHIC_V0));
  init_ui_element (&s_tutorial_graphic, (TUTO_GRAPHIC_WIDTH),
                   (TUTO_GRAPHIC_WIDTH), (TUTO_GRAPHIC_X_POS),
                   (TUTO_GRAPHIC_Y_POS), (TUTO_GRAPHIC_V0));
  init_ui_element (&s_game_over_score_box, (GAME_OVER_SCORE_BOX_WIDTH),
                   (GAME_OVER_SCORE_BOX_HEIGHT), (GAME_OVER_SCORE_BOX_X_POS),
                   (GAME_OVER_SCORE_BOX_Y_POS), (GAME_OVER_SCORE_BOX_V0));
  init_ui_element (&s_silver_medal, (MEDAL_WIDTH), (MEDAL_HEIGHT),
                   (MEDAL_X_POS), (MEDAL_Y_POS), (MEDALS_V0));
  init_ui_element (&s_gold_medal, (MEDAL_WIDTH), (MEDAL_HEIGHT), (MEDAL_X_POS),
                   (MEDAL_Y_POS), (MEDALS_V0));

  s_gold_medal.u0 = (GOLD_MEDAL_U0);

  memset (s_score_normal_text, 0, sizeof (SPRT) * (SCORE_MAX_NUM_DIGITS));
  memset (s_score_game_over_text, 0, sizeof (SPRT) * (SCORE_MAX_NUM_DIGITS));
  memset (s_high_score_text, 0, sizeof (SPRT) * (SCORE_MAX_NUM_DIGITS));

  for (i = 0; i < (SCORE_MAX_NUM_DIGITS); i++)
  {
    init_score_text_element (&s_score_normal_text[i]);
    setXY0 (&s_score_normal_text[i], (SCORE_TEXT_X_POS), (SCORE_TEXT_Y_POS));
    setUV0 (&s_score_normal_text[i], 0, (SCORE_TEXT_V0));
    setWH (&s_score_normal_text[i], (SCORE_TEXT_WIDTH), (SCORE_TEXT_HEIGHT));

    init_score_text_element (&s_score_game_over_text[i]);
    setXY0 (&s_score_game_over_text[i], (SCORE_TEXT_GAME_OVER_X_POS),
            (SCORE_TEXT_GAME_OVER_Y_POS));
    setUV0 (&s_score_game_over_text[i], 0, (SCORE_TEXT_GAME_OVER_V0));
    setWH (&s_score_game_over_text[i], (SCORE_TEXT_GAME_OVER_WIDTH),
           (SCORE_TEXT_GAME_OVER_HEIGHT));

    init_score_text_element (&s_high_score_text[i]);
    setXY0 (&s_high_score_text[i], (HIGH_SCORE_TEXT_X_POS),
            (HIGH_SCORE_TEXT_Y_POS));
    setUV0 (&s_high_score_text[i], 0, (SCORE_TEXT_GAME_OVER_V0));
    setWH (&s_high_score_text[i], (SCORE_TEXT_GAME_OVER_WIDTH),
           (SCORE_TEXT_GAME_OVER_HEIGHT));
  }

  u8_num_digits = 1;

  SetPolyF4 (&pf4_pause_overlay);
  setRGB0 (&pf4_pause_overlay, 0, 0, 0);
  SetSemiTrans (&pf4_pause_overlay, 1);
  SetShadeTex (&pf4_pause_overlay, 1);
  setXYWH (&pf4_pause_overlay, 0, 0, (FB_SCREEN_WIDTH), (FB_SCREEN_HEIGHT));
}

void
init_ui_element (SPRT* sptr_element, const uint16_t u16_width,
                 const uint16_t u16_height, const uint16_t u16_x0,
                 const uint16_t u16_y0, const uint16_t u16_v0)
{
  SetSprt (sptr_element);
  SetShadeTex (sptr_element, 1);
  setWH (sptr_element, u16_width, u16_height);
  setUV0 (sptr_element, 0, u16_v0);
  setXY0 (sptr_element, u16_x0, u16_y0);
  sptr_element->clut = texture_clut_lookup[TID_UI_TEXTURE];
}

void
init_score_text_element (SPRT* sptr_element)
{
  SetSprt (sptr_element);
  SetShadeTex (sptr_element, 1);
  sptr_element->clut = texture_clut_lookup[TID_FONT_TEXTURE];
}

void
ui_draw_ui_elements (u_long *ot, u_long *ot_idx)
{
#ifdef DEBUG_BUILD
  assert((*ot_idx) < (FB_ORDERING_TABLE_MAX_LENGTH));
#endif /* DEBUG_BUILD */
  AddPrim(&ot[(*ot_idx)], &tpages[TID_UI_TEXTURE]);
  (*ot_idx)++;

  switch (gs_get_curr_game_state ())
  {
    case GSTATE_GAME_START:
      draw_ui_game_start (ot, ot_idx);
      break;
    case GSTATE_NORMAL:
      draw_ui_normal (ot, ot_idx);
      break;
    case GSTATE_GAME_OVER:
      draw_ui_game_over (ot, ot_idx);
      break;
    case GSTATE_GAME_PAUSED:
      draw_ui_game_paused (ot, ot_idx);
      break;
    default:
      break;
  }
}

void
draw_ui_game_start (u_long *ot, u_long *ot_idx)
{
  draw_ui_element(ot, ot_idx, &s_get_ready_graphic);
  draw_ui_element(ot, ot_idx, &s_tutorial_graphic);
}

void
draw_ui_normal (u_long *ot, u_long *ot_idx)
{
  add_font_tpage_to_ot (ot, ot_idx);
  draw_ui_score_text (ot, ot_idx, s_score_normal_text);
}

void
add_font_tpage_to_ot (u_long *ot, u_long *ot_idx)
{
#ifdef DEBUG_BUILD
  assert((*ot_idx) < (FB_ORDERING_TABLE_MAX_LENGTH));
#endif /* DEBUG_BUILD */
  AddPrim(&ot[(*ot_idx)], &tpages[TID_FONT_TEXTURE]);
  (*ot_idx)++;
}

void
draw_ui_score_text (u_long *ot, u_long *ot_idx, SPRT *sptr_score_text)
{
  uint8_t i;

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
draw_ui_high_score_text (u_long *ot, u_long *ot_idx, SPRT *sptr_score_text)
{
  uint8_t i;

  for (i = 0; i < u8_high_score_num_digits; i++)
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
  draw_ui_element(ot, ot_idx,   &s_game_over_graphic);
  draw_ui_element(ot, ot_idx, &s_game_over_score_box);

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

  add_font_tpage_to_ot (ot, ot_idx);

  draw_ui_score_text (ot, ot_idx, s_score_game_over_text);
  draw_ui_high_score_text (ot, ot_idx, s_high_score_text);
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

void
ui_update_current_score_text (uint16_t u16_score)
{
  uint16_t i;
  uint16_t u16_score_cpy;

  u8_num_digits = calc_num_digits (u16_score);

  switch (u8_num_digits)
  {
    case 1:
      s_score_normal_text[0].x0 = (SCORE_TEXT_X_POS);

      s_score_game_over_text[0].x0 = (SCORE_TEXT_GAME_OVER_X_POS);
      break;
    case 2:
      s_score_normal_text[0].x0 =
        (SCORE_TEXT_X_POS) - (SCORE_TEXT_HALF_WIDTH);
      s_score_normal_text[1].x0 =
        (SCORE_TEXT_X_POS) + (SCORE_TEXT_HALF_WIDTH);

      s_score_game_over_text[0].x0 =
        (SCORE_TEXT_GAME_OVER_X_POS) - (SCORE_TEXT_GAME_OVER_WIDTH);
      s_score_game_over_text[1].x0 = (SCORE_TEXT_GAME_OVER_X_POS);
      break;
    case 3:
      s_score_normal_text[0].x0 =
        (SCORE_TEXT_X_POS) - (SCORE_TEXT_WIDTH);
      s_score_normal_text[1].x0 = (SCORE_TEXT_X_POS);
      s_score_normal_text[2].x0 =
        (SCORE_TEXT_X_POS) + (SCORE_TEXT_WIDTH);

      s_score_game_over_text[0].x0 =
        (SCORE_TEXT_GAME_OVER_X_POS) - ((SCORE_TEXT_GAME_OVER_WIDTH) << 1);
      s_score_game_over_text[1].x0 =
        (SCORE_TEXT_GAME_OVER_X_POS) - (SCORE_TEXT_GAME_OVER_WIDTH);
      s_score_game_over_text[2].x0 = (SCORE_TEXT_GAME_OVER_X_POS);
      break;
    default:
      break;
  }

  u16_score_cpy = u16_score;
  for (i = u8_num_digits; i != 0; i--)
  {
    s_score_normal_text[i - 1].u0 = (u16_score_cpy % 10) * (SCORE_TEXT_WIDTH);
    s_score_game_over_text[i - 1].u0 = \
      (u16_score_cpy % 10) * (SCORE_TEXT_GAME_OVER_WIDTH);
    u16_score_cpy /= 10;
  }
}

uint8_t
calc_num_digits (uint16_t x)
{
  uint16_t i;
  uint8_t u8_output = 0;

  for (i = x; i != 0; i /= 10)
    u8_output++;

  if (u8_output > 0)
  {
    if (u8_output >= (SCORE_MAX_NUM_DIGITS))
    {
      return (SCORE_MAX_NUM_DIGITS);
    }

    return u8_output;
  }

  return 1;
}

void
ui_update_high_score_text (uint16_t u16_high_score)
{
  uint16_t i;
  uint16_t u16_high_score_cpy;

  u8_high_score_num_digits = calc_num_digits (u16_high_score);

  switch (u8_high_score_num_digits)
  {
    case 1:
      s_high_score_text[0].x0 = (SCORE_TEXT_GAME_OVER_X_POS);
      break;
    case 2:
      s_high_score_text[0].x0 = \
        (SCORE_TEXT_GAME_OVER_X_POS) - (SCORE_TEXT_GAME_OVER_WIDTH);
      s_high_score_text[1].x0 = (SCORE_TEXT_GAME_OVER_X_POS);
      break;
    case 3:
      s_high_score_text[0].x0 = \
        (SCORE_TEXT_GAME_OVER_X_POS) - ((SCORE_TEXT_GAME_OVER_WIDTH) << 1);
      s_high_score_text[1].x0 = \
        (SCORE_TEXT_GAME_OVER_X_POS) - (SCORE_TEXT_GAME_OVER_WIDTH);
      s_high_score_text[2].x0 = (SCORE_TEXT_GAME_OVER_X_POS);
      break;
    default:
      break;
  }

  u16_high_score_cpy = u16_high_score;
  for (i = u8_high_score_num_digits; i != 0; i--)
  {
    s_high_score_text[i - 1].u0 = \
      (u16_high_score_cpy % 10) * (SCORE_TEXT_GAME_OVER_WIDTH);
    u16_high_score_cpy /= 10;
  }
}

void
draw_ui_game_paused (u_long *ot, u_long *ot_idx)
{
  add_font_tpage_to_ot (ot, ot_idx);
  draw_ui_score_text (ot, ot_idx, s_score_normal_text);

  AddPrim (&ot[(*ot_idx)], &pf4_pause_overlay);
  (*ot_idx)++;
}
