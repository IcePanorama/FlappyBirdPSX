#ifndef _SCREEN_WRITER_H_
#define _SCREEN_WRITER_H_

#define SW_FONT_TPAGE_X (320)
#define SW_FONT_TPAGE_Y (0)

#define SW_FONT_CLUT_X (0)
#define SW_FONT_CLUT_Y (480)

#define SW_FONT_TEXTURE_WIDTH (256)
#define SW_FONT_TEXTURE_HEIGHT (64)

#define SW_MAX_NUM_GLYPHS (256)
#define SW_MAX_NUM_TEXT_OUTPUTS (32)

#include "sys/fb_ints.h"

typedef struct TextOutput_s
{
  uint8_t u8_id;
  uint8_t u8_length;
  char zcs_msg[(SW_MAX_NUM_GLYPHS)];
  SPRT_8 glyphs[(SW_MAX_NUM_GLYPHS)];
} TextOutput_t;

/*
 *  MUST INSERT DR_TPAGE oR DR_MODE prim into ot prior to adding all of these,
 *  specifying the font texture page.
 */
extern TextOutput_t sw_output_pool[(SW_MAX_NUM_TEXT_OUTPUTS)];
extern uint8_t sw_num_outputs;

void sw_init (void);
//void sw_print_char (const char c, uint16_t x, uint16_t y);
uint8_t sw_print (const char *str, uint16_t x, uint16_t y);

#endif /* _SCREEN_WRITER_H_ */
