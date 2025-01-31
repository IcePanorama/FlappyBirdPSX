#include "video/textmngr.h"
#include "sys/fb_defs.h"
#include "sys/fb_ints.h"
#include "video/textlkup.h"

#include <libcd.h>

#ifdef DEBUG_BUILD
  #include <assert.h>
  #include <stdio.h>
#endif /* DEBUG_BUILD */

#define CLUT_STARTING_X_VALUE (0)
#define CLUT_STARTING_Y_VALUE ((FB_SCREEN_HEIGHT) << 1)

#define TPAGE_STARTING_X_VALUE (1024)
#define TPAGE_STARTING_Y_VALUE (0)
#define TPAGE_HEIGHT (256)

#define FILE_MAX_NUM_SECTORS (25)
#define FILE_MAX_SIZE_BYTES ((FILE_MAX_NUM_SECTORS) << 11)

#define VRAM_WIDTH (1024)
#define VRAM_HEIGHT (512)

/**
 *  Aligns a `ptr` to the nearest byte, relative to `start`.
 *  Params should be `u_short **, char *`.
 */
#define ALIGN_PTR_TO_NEAREST_BYTE(ptr, start) \
  while ((((char *)(*(ptr)) - (start))) % 4 != 0) \
    (*(ptr))++;


DR_TPAGE tpages[TID_NUM_TEXTURES] = {{0}};
DR_TPAGE player_tpage = {0};

static void load_texture (const char *path);

/**
 *  Loads the clut pointed to by `data_ptr` into VRAM. Starts at
 *  `(CLUT_STARTING_X_VALUE, CLUT_STARTING_Y_VALUE)` and automatically adjusts
 *  its internal x, y values as needed. Automatically grows from that starting
 *  position to `(VRAM_WIDTH, VRAM_HEIGHT)`.
 *
 *  param:  `data_start`  beginning of file data, needed for data alignment.
 *  param:  `data_ptr`    beginning of a CLUT data pack entry.
 *  See:
 *    https://www.github.com/IcePanorama/PSXBMPPacker/blob/main/include/color_lookup_table.hpp
 */
static void process_clut (char *data_start, u_short **data_ptr, TextureID_t tid);

/**
 *  Loads the pixel data pointed to by `data_ptr` into VRAM. Starts at
 *  `(TPAGE_STARTING_X_VALUE, TPAGE_STARTING_Y_VALUE)` and automatically adjusts
 *  its internal x, y values as needed. Automatically grows backwards from that
 *  starting position to `(FB_SCREEN_WIDTH, CLUT_STARTING_Y_VALUE - 1)`.
 *
 *  param:  `data_start`  beginning of file data, needed for data alignment.
 *  param:  `data_ptr`    beginning of a pixel-data data pack entry.
 *  See:
 *    https://www.github.com/IcePanorama/PSXBMPPacker/blob/main/include/pixel_data.hpp
 */
static void process_pixel_data (char *data_start, u_short **data_ptr, TextureID_t tid);

void
tmg_auto_load_textures (void)
{
  load_texture("\\ASSETS\\FONT.DAT;1");
  load_texture("\\ASSETS\\SPRITES.DAT;1");
}

void
load_texture (const char *path)
{
  CdlFILE fptr;
  u_long sprite_data[(FILE_MAX_SIZE_BYTES)] = {0};
  u_short *work;
  uint16_t i;
  uint8_t u8_type;
  uint8_t u8_entry_id;
  uint16_t u16_num_entries = 0;

  /*
   *  TODO: save the location of these files somewhere so we don't have to
   *  actually search for it on boot.
   *  Also write a non-debug version of this without all the asserts.
   */
#ifdef DEBUG_BUILD
  assert(CdSearchFile (&fptr, (char *)path) != 0);
//  printf("Size: %ld\n", fptr.size);
  assert(fptr.size <= (FILE_MAX_SIZE_BYTES));
  assert(CdControlB (CdlSetloc, (u_char *)&fptr.pos, 0) != 0);
  assert(CdRead ((FILE_MAX_NUM_SECTORS), sprite_data, CdlModeSpeed) != 0);
#else /* DEBUG_BUILD */
  if ((CdSearchFile (&fptr, (char *)path) == 0)
      || (CdControlB (CdlSetloc, (u_char *)&fptr.pos, 0) == 0)
      || (CdRead ((FILE_MAX_NUM_SECTORS), sprite_data, CdlModeSpeed) == 0))
  {
    return;
  }
#endif /* DEBUG_BUILD */
  CdReadSync(0, 0); // wait for operation to finish.

  work = (u_short *)(&sprite_data[0]);
  u16_num_entries = *work;
  work++;
  for (i = 0; i < u16_num_entries; i++)
  {
    u8_entry_id = (*work) & 0xFF;
#ifdef DEBUG_BUILD
    assert(u8_entry_id < TID_NUM_TEXTURES);
#endif /* DEBUG_BUILD */

    u8_type = (((*work) & 0xFF00) >> 8);
    work++;

    if ((u8_type & 1) == 0)
      process_clut ((char *)(&sprite_data[0]), &work, u8_entry_id);
    else
      process_pixel_data ((char *)(&sprite_data[0]), &work, u8_entry_id);
  }
}

void
process_clut (char *data_start, u_short **data_ptr, TextureID_t tid)
{
  static uint16_t u16_clut_x = (CLUT_STARTING_X_VALUE);
  static uint16_t u16_clut_y = (CLUT_STARTING_Y_VALUE);
  uint16_t u16_clut_width;
  uint16_t u16_clut_height;

#ifdef DEBUG_BUILD
    assert(u16_clut_x < 1024);
#endif /* DEBUG_BUILD */

  u16_clut_width = **data_ptr;
  (*data_ptr)++;

  u16_clut_height = **data_ptr;
  (*data_ptr)++;

  ALIGN_PTR_TO_NEAREST_BYTE(data_ptr, data_start);

  texture_clut_lookup[tid] =
    LoadClut2((u_long *)(*data_ptr), u16_clut_x, u16_clut_y);
  DrawSync (0);
  (*data_ptr) += 16; // skip over clut

  u16_clut_y += u16_clut_height;
  if (u16_clut_y > (VRAM_HEIGHT))
  {
    u16_clut_x += u16_clut_width;
    /**
     *  Rounds `u16_clut_x` up to the nearest multiple of 16.
     *  Without bitshifts: `((u16_clut_x + 15) / 16) * 16`.
     */
    u16_clut_x = ((u16_clut_x + 15) >> 4) << 4;
    u16_clut_y = (CLUT_STARTING_Y_VALUE);
  }
}

void
process_pixel_data (char *data_start, u_short **data_ptr, TextureID_t tid)
{
  static uint16_t u16_tpage_x = (TPAGE_STARTING_X_VALUE);
  static uint16_t u16_tpage_y = (TPAGE_STARTING_Y_VALUE);
  RECT rect;

#ifdef DEBUG_BUILD
  assert(u16_tpage_y < 512);
#endif /* DEBUG_BUILD */

  rect.w = **data_ptr;
  (*data_ptr)++;
  rect.h = **data_ptr;
  (*data_ptr)++;

  do
  {
    u16_tpage_x -= rect.w;
    /**
     *  Rounds `u16_tpage_x` down to nearest multiple of 64.
     *  Without bitshifts: `((u16_tpage_x - 63) / 64) * 64`.
     */
    u16_tpage_x = ((u16_tpage_x - 63) >> 6) << 6;
    if (u16_tpage_x < (FB_SCREEN_WIDTH))
    {
      u16_tpage_y += (TPAGE_HEIGHT);
      u16_tpage_x = (TPAGE_STARTING_X_VALUE);
    }
  }
  while ((u16_tpage_x < (FB_SCREEN_WIDTH)) || (u16_tpage_x >= (VRAM_WIDTH)));

  rect.x = u16_tpage_x;
  rect.y = u16_tpage_y;

  ALIGN_PTR_TO_NEAREST_BYTE(data_ptr, data_start);

  LoadImage(&rect, (u_long *)(*data_ptr));
  DrawSync (0);

  u16_tpage_y += rect.y;

  texture_tpage_lookup[tid] = GetTPage (0, 0, rect.x, rect.y);
  SetDrawTPage (&tpages[tid], 0, 0, texture_tpage_lookup[tid]);
  (*data_ptr) += (rect.w * rect.h);
}
