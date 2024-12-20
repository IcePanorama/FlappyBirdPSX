#ifndef _TEXTURE_LOOKUP_H_
#define _TEXTURE_LOOKUP_H_

#include <sys/types.h>

typedef enum TextureID_e
{
  TEXTID_FONT_TEXTURE,
  TEXTID_PIPE_BOT_TEXTURE,
  TEXTID_TMP,
  TEXTID_NUM_TEXTURES
} TextureID_t;

extern u_short texture_tpage_lookup[TEXTID_NUM_TEXTURES];
extern u_short texture_clut_lookup[TEXTID_NUM_TEXTURES];

#endif /* _TEXTURE_LOOKUP_H_ */
