/* Don't touch! CC is very particular about the order of all of these. */
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libcd.h>

#include "game/scrwritr.h"

#include <string.h>

#ifdef DEBUG_BUILD
#include "sys/fb_bools.h"

#include <assert.h>
#include <stdio.h>
#endif /* DEBUG_BUILD */

#define FONT_FILE_NUM_SECTORS (5)
#define FONT_FILE_DATA_SIZE ((FONT_FILE_NUM_SECTORS) << 11)

#define FONT_SPRITE_WIDTH (8)
#define FONT_SPRITE_HEIGHT (8)
#define FONT_SPRITE_HALF_WIDTH ((FONT_SPRITE_WIDTH) >> 1)
#define FONT_SPRITE_HALF_HEIGHT ((FONT_SPRITE_HEIGHT) >> 1)

TextOutput_t sw_output_pool[(SW_MAX_NUM_TEXT_OUTPUTS)] = {{0}};
uint8_t sw_num_outputs = 0;

static void load_font (void);
static uint8_t create_new_text_output (void);
static TextOutput_t *get_text_output (uint8_t u8_id);

static u_short FONT_CLUT_ID;

void
sw_init (void)
{
  memset (sw_output_pool, 0, sizeof (TextOutput_t) * (SW_MAX_NUM_TEXT_OUTPUTS));
  sw_num_outputs = 0;

  load_font ();
  FONT_CLUT_ID = GetClut ((SW_FONT_CLUT_X), (SW_FONT_CLUT_Y));
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

// FIXME: NEEDS TO BE FINISHED!!!
uint8_t
sw_print (const char *str, uint16_t x, uint16_t y)
{
  uint8_t u8_output;
  TextOutput *to;
  uint16_t u16_top_left_x;
  uint16_t u16_top_left_y;
  char *cptr;

  u8_output = create_new_text_output ();
  to = get_text_output (u8_output);
  to->u8_length = strlen (str);
#ifdef DEBUG_BUILD
  assert (to->u8_length < (SW_MAX_NUM_GLYPHS));
#endif /* DEBUG_BUILD */

  u16_top_left_x = x - (to->u8_length >> 1);
  u16_top_left_y = y - (to->u8_length * (FONT_SPRITE_WIDTH) % (SCREEN_WIDTH));

  strncpy(to->zcs_msg, str, to->u8_length);
  for (i = 0, cptr = str; i < to->u8_length; i++, cptr++, u16_top_left_x += 8) // handle line breaks
  {
    SetSprt8(&to->glyphs[i]);
    setXY0(&to->glyphs[i], u16_top_left_x, u16_top_left_y);
    to->glyphs[i].clut = FONT_CLUT_ID;
    if ('A' <= *cptr && *cptr <= 'Z')  // todo: handle uppercase letters.
    {
    }
    else if ('a' <= *cptr && *cptr <= 'z')  // todo: handle lowercase letters
    {
    }
    else if ('0' <= *cptr && *cptr <= '9')  // todo: handle numerals
    {
    }
    else  // all else
  {
      switch (*cptr)
      {
        case '!':
          break;
        case '.':
        default:
          break;
      }
    }
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
