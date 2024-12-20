#ifndef _SCREEN_WRITER_H_
#define _SCREEN_WRITER_H_

#define SW_FONT_TPAGE_X (320)
#define SW_FONT_TPAGE_Y (0)

#define SW_FONT_CLUT_X (0)
#define SW_FONT_CLUT_Y (480)

#define SW_FONT_TEXTURE_WIDTH (256)
#define SW_FONT_TEXTURE_HEIGHT (64)

#define SW_FONT_SPRITE_HEIGHT (16)

#define SW_MAX_NUM_GLYPHS (255)
#define SW_MAX_NUM_TEXT_OUTPUTS (32)

/* Don't touch! CC is very particular about the order of all of these. */
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>

#include "sys/fb_ints.h"

typedef struct TextOutput_s
{
  uint8_t u8_id;
  uint8_t u8_length;
  char zcs_msg[(SW_MAX_NUM_GLYPHS)];
  SPRT_16 glyphs[(SW_MAX_NUM_GLYPHS)];
} TextOutput_t;

/*
 *  MUST insert a DR_TPAGE oR DR_MODE prim into the ot prior to adding all of
 *  these, specifying the font texture page.
 */
extern TextOutput_t sw_output_pool[(SW_MAX_NUM_TEXT_OUTPUTS)];
extern uint8_t sw_num_outputs;
//extern u_short sw_font_tpage_id;

void sw_init (void);
//void sw_print_char (const char c, uint16_t x, uint16_t y);
/**
 *  FntPrint for preformatted strings, prints them using the game's custom font.
 *  Caller must also call `sw_destroy_text_output` to free your message when you
 *  wish to remove some prior message.
 *  param   str  some null terminated cstring.
 *  param   x    x position of the message's center (in screen space)
 *  param   y    y position of the message's center (in screen space)
 *  return  u8 of the messages id (needed for destroying text later)
 */
uint8_t sw_print (const char *str, int16_t x, int16_t y);
void sw_destroy_text_output (uint8_t u8_id);

#endif /* _SCREEN_WRITER_H_ */
