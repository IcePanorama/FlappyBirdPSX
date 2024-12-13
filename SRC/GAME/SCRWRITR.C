#include "game/scrwritr.h"
#include "sys/fb_defs.h"

#include <libcd.h>
#include <string.h>

#ifdef DEBUG_BUILD
#include "sys/fb_bools.h"

#include <assert.h>
#include <stdio.h>
#endif /* DEBUG_BUILD */

#define FONT_FILE_NUM_SECTORS (5)
#define FONT_FILE_DATA_SIZE ((FONT_FILE_NUM_SECTORS) << 11)

#define FONT_SPRITE_WIDTH (16)
#define FONT_SPRITE_HALF_WIDTH ((FONT_SPRITE_WIDTH) >> 1)
#define FONT_SPRITE_HALF_HEIGHT ((SW_FONT_SPRITE_HEIGHT) >> 1)

TextOutput_t sw_output_pool[(SW_MAX_NUM_TEXT_OUTPUTS)] = {{0}};
uint8_t sw_num_outputs = 0;
u_short sw_font_tpage_id;

static void load_font (void);
static uint8_t create_new_text_output (void);
static TextOutput_t *get_text_output (uint8_t u8_id);
// TODO: create macro version of this!
static uint8_t calculate_glyph_idx (char c);

static u_short FONT_CLUT_ID;

void
sw_init (void)
{
  memset (sw_output_pool, 0, sizeof (TextOutput_t) * (SW_MAX_NUM_TEXT_OUTPUTS));
  sw_num_outputs = 0;

  load_font ();
  FONT_CLUT_ID = GetClut ((SW_FONT_CLUT_X), (SW_FONT_CLUT_Y));
  sw_font_tpage_id = GetTPage (0, 0, (SW_FONT_TPAGE_X), (SW_FONT_TPAGE_Y));
}

void
load_font (void)
{
  CdlFILE fptr;
  u_long font_data[(FONT_FILE_DATA_SIZE)] = {0};
  TIM_IMAGE font_img;

#ifdef DEBUG_BUILD
  CdSetDebug (3);

  assert(CdInit() != 0);
  assert(CdSearchFile (&fptr, "\\ASSETS\\FONT.TIM;1") != 0);
  assert(CdControlB (CdlSetloc, (u_char *)&fptr.pos, 0) != 0);
  assert(CdRead ((FONT_FILE_NUM_SECTORS), font_data, CdlModeSpeed) != 0);
#else /* DEBUG_BUILD */
  if (CdInit() == 0 || CdSearchFile (&fptr, "\\ASSETS\\FONT.TIM;1") == 0 ||
    CdControlB (CdlSetloc, (u_char *)&fptr.pos, 0) == 0 ||
    CdRead ((FONT_FILE_NUM_SECTORS), font_data, CdlModeSpeed) == 0)
  {
    exit (-1);
  }
#endif /* DEBUG_BUILD */
  CdReadSync(0, 0); // wait for operation to finish.

  assert(OpenTIM (font_data) == 0);
  assert(ReadTIM (&font_img) != 0);
  LoadImage (font_img.crect, font_img.caddr);
  DrawSync (0);
  LoadImage (font_img.prect, font_img.paddr);
  DrawSync (0);
}

uint8_t
sw_print (const char *str, int16_t x, int16_t y)
{
  uint8_t u8_output;
  TextOutput_t *to;
  int16_t i16_top_left_x;
  int16_t i16_top_left_y;
  uint16_t i8_message_width;
  uint8_t i;
  const char *cptr;
  uint8_t u8_sprt_idx;

  u8_output = create_new_text_output ();
  to = get_text_output (u8_output);
  to->u8_length = strlen (str);
#ifdef DEBUG_BUILD
  assert (to->u8_length < (SW_MAX_NUM_GLYPHS));
#endif /* DEBUG_BUILD */

  i8_message_width = to->u8_length * (FONT_SPRITE_WIDTH);
  i16_top_left_x = x - (i8_message_width >> 1);
  i16_top_left_y = y - FONT_SPRITE_HALF_HEIGHT;

  strncpy(to->zcs_msg, str, to->u8_length);
  for (i = 0, cptr = str; cptr < str + to->u8_length; i++, cptr++,
  i16_top_left_x += (FONT_SPRITE_WIDTH))
  {
    if (*cptr == ' ') continue;
    u8_sprt_idx = calculate_glyph_idx (*cptr);

    SetSprt16(&to->glyphs[i]);
    setXY0(&to->glyphs[i], i16_top_left_x, i16_top_left_y);
    setRGB0(&to->glyphs[i], 0xFF, 0xFF, 0xFF);
    to->glyphs[i].clut = FONT_CLUT_ID;
#ifdef DEBUG_BUILD
    assert(u8_sprt_idx < (SW_FONT_TEXTURE_HEIGHT));
#endif /* DEBUG_BUILD */
    setUV0(&to->glyphs[i],
           (u8_sprt_idx * (FONT_SPRITE_WIDTH) % (SW_FONT_TEXTURE_WIDTH)),
           (u_char)(u8_sprt_idx / (SW_FONT_SPRITE_HEIGHT)) * (SW_FONT_SPRITE_HEIGHT)
           );
  }

  return u8_output;
}

uint8_t
create_new_text_output (void)
{
  static uint8_t u8_num_outputs_created = 0; // count incl. deleted ones
  const uint8_t u8_OUTPUT = u8_num_outputs_created;

#ifdef DEBUG_BUILD
  assert((sw_num_outputs + 1) < (SW_MAX_NUM_TEXT_OUTPUTS));
#endif /* DEBUG_BUILD */

  sw_output_pool[sw_num_outputs].u8_id = u8_OUTPUT;
  sw_num_outputs++;
  u8_num_outputs_created++;
  return u8_OUTPUT;
}

TextOutput_t *
get_text_output (uint8_t u8_id)
{
  uint8_t i;

#ifdef DEBUG_BUILD
  assert (sw_num_outputs != 0);
#endif /* DEBUG_BUILD */

  for (i = 0; i < sw_num_outputs; i++)
  {
    if (sw_output_pool[i].u8_id == u8_id)
      return &sw_output_pool[i];
  }

#ifdef DEBUG_BUILD
  printf ("ERROR: TextOutput with id, %d, not found!\n", u8_id);
  assert (FALSE);
#endif /* DEBUG_BUILD */

  return 0;
}

uint8_t
calculate_glyph_idx (char c)
{
  if ('A' <= c && c <= 'Z')
  {
    return c - 'A';
  }
  else if ('a' <= c && c <= 'z')
  {
    return 26 + (c - 'a');
  }
  else if ('0' <= c && c <= '9')
  {
    return 52 + (c - '0');
  }
  else
{
    switch (c)
    {
      case '.':
        return 62;
      case '!':
      default:
        return 63;
    }
  }
}

void
sw_destroy_text_output (uint8_t u8_id)
{
  TextOutput_t to_tmp;
  uint8_t i;
  uint8_t u8_idx = -1;

  for (i = 0; i < sw_num_outputs; i++)
  {
    if (sw_output_pool[i].u8_id == u8_id)
    {
      u8_idx = i;
      break;
    }
  }

#ifdef DEBUG_BUILD
  assert(u8_idx != (uint8_t)-1);
#endif /* DEBUG_BUILD */

  if (u8_idx != (sw_num_outputs - 1))
  {
    to_tmp = sw_output_pool[u8_idx];
    sw_output_pool[u8_idx] = sw_output_pool[sw_num_outputs - 1];
    sw_output_pool[sw_num_outputs - 1] = to_tmp;
  }

  sw_num_outputs--;
}
