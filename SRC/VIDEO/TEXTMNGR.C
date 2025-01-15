#include "video/textmngr.h"
#include "sys/fb_defs.h"
#include "sys/fb_ints.h"
#include "video/textlkup.h"

#include <libcd.h>

#ifdef DEBUG_BUILD
  #include <assert.h>
  #include <stdio.h>
#endif /* DEBUG_BUILD */

#define CLUT_STARTING_X_VALUE (1024)
#define CLUT_STARTING_Y_VALUE (512)

#define TPAGE_STARTING_X_VALUE (1024)
#define TPAGE_STARTING_Y_VALUE (0)

//TODO: rename.
#define SPRITES_NUM_SECTORS (20)
#define SPRITES_SIZE_BYTES ((SPRITES_NUM_SECTORS) << 11)

/**
 *  Aligns a `ptr` to the nearest byte, relative to `start`. Params should be
 *  `u_short **, char *`.
 */
#define ALIGN_PTR_TO_NEAREST_BYTE(ptr, start) \
  (*(ptr)) += (4 - (((char *)(*(ptr)) - (start)) % 4)) % 4

DR_TPAGE tpages[TEXTID_NUM_TEXTURES] = {{0}};
DR_TPAGE player_tpage = {0};

static void load_texture (const char *path);

/**
 *  `data_start` points to the absolute beginning of file data and is used
 *  purely for data alignment. `data_ptr` should point to the beginning of a
 *  CLUT data pack entry.
 *
 *  See:
 *    https://www.github.com/IcePanorama/PSXBMPPacker/blob/main/include/color_lookup_table.hpp
 */
static void process_clut (char *data_start, u_short **data_ptr,
                          TextureID_t tid);

/**
 *  `data_start` points to the absolute beginning of file data and is used
 *  purely for data alignment. `data_ptr` should point to the beginning of a
 *  pixel-data data pack entry.
 *
 *  See:
 *    https://www.github.com/IcePanorama/PSXBMPPacker/blob/main/include/pixel_data.hpp
 */
static void process_pixel_data (char *data_start, u_short **data_ptr,
                                TextureID_t tid);

void
tmg_auto_load_textures (void)
{
  load_texture("\\ASSETS\\SPRITES.DAT;1");
  load_texture("\\ASSETS\\FONT.DAT;1");
  load_texture("\\ASSETS\\PIPES.DAT;1");
}

void
load_texture (const char *path)
{
  CdlFILE fptr;
  u_long sprite_data[(SPRITES_SIZE_BYTES)] = {0};
  u_short *work;
  uint16_t i;
  uint8_t u8_type;
  uint8_t u8_entry_id;
  uint16_t u16_num_entries = 0;

  /*
   *  TODO: save the location of these files somewhere so we don't have to
   *  actually search for it on boot.
   */
  assert(CdSearchFile (&fptr, (char *)path) != 0);
  assert(fptr.size <= (SPRITES_SIZE_BYTES));
  assert(CdControlB (CdlSetloc, (u_char *)&fptr.pos, 0) != 0);
  assert(CdRead ((SPRITES_NUM_SECTORS), sprite_data, CdlModeSpeed) != 0);
  CdReadSync(0, 0); // wait for operation to finish.

  work = (u_short *)(&sprite_data[0]);
  u16_num_entries = *work;
  work++;
  for (i = 0; i < u16_num_entries; i++)
  {
    u8_entry_id = (*work) & 0xFF;

    printf("%d\n", u8_entry_id);
    printf("%04X\n", *work);
    printf("%d\n",TEXTID_NUM_TEXTURES);
    printf("%d\n", (char *)work - (char *)(&sprite_data[0]));
#ifdef DEBUG_BUILD
    assert(u8_entry_id < TEXTID_NUM_TEXTURES);
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

  u16_clut_x -= **data_ptr;
  (*data_ptr)++;

  u16_clut_y -= **data_ptr;
  (*data_ptr)++;

  ALIGN_PTR_TO_NEAREST_BYTE(data_ptr, data_start);

  texture_clut_lookup[tid] =
    LoadClut2((u_long *)(*data_ptr), u16_clut_x, u16_clut_y);
  DrawSync (0);
  (*data_ptr) += 16; // skip over clut
}

void
process_pixel_data (char *data_start, u_short **data_ptr, TextureID_t tid)
{
  static uint16_t u16_tpage_x = (TPAGE_STARTING_X_VALUE);
  static uint16_t u16_tpage_y = (TPAGE_STARTING_Y_VALUE);
  RECT rect;

  rect.w = **data_ptr;
  (*data_ptr)++;
  rect.h = **data_ptr;
  (*data_ptr)++;

  //TODO: handle collisions in the y direction.
  u16_tpage_x -= rect.w;
  /**
   *  Rounds `u16_tpage_x` down to nearest multiple of 64. Without bitshifts:
   *  `((u16_tpage_x - 63) / 64) * 64`.
   */
  u16_tpage_x = ((u16_tpage_x - 63) >> 6) << 6;
  if (u16_tpage_x < SCREEN_WIDTH)
  {
    u16_tpage_y += 256;
    u16_tpage_x =(TPAGE_STARTING_X_VALUE);
  }
  rect.x = u16_tpage_x;
  rect.y = u16_tpage_y;

  ALIGN_PTR_TO_NEAREST_BYTE(data_ptr, data_start);

  LoadImage(&rect, (u_long *)(*data_ptr));
  DrawSync (0);

  u16_tpage_y += rect.y;

  texture_tpage_lookup[tid] = GetTPage (0, 0, rect.x, rect.y);
  SetDrawTPage (&tpages[tid], 0, 0, texture_tpage_lookup[tid]);
  //FIXME: issue is coming from here, I believe.
  (*data_ptr) += (rect.w * rect.h);
}
