#ifndef _TEXTURE_ID_H_
#define _TEXTURE_ID_H_

// FIXME: prefix should be TID instead of TEXTID
typedef enum TextureID_e
{
  TEXTID_FONT_TEXTURE,
  TEXTID_PIPES_TEXTURE,
  TEXTID_BG_TEXTURE,
  TEXTID_FG_TEXTURE,
  TEXTID_PLAYER_TEXTURE,
  TEXTID_UI_TEXTURE
//TODO: do something similar to this elsewhere where possible.
#ifdef DEBUG_BUILD
, TEXTID_NUM_TEXTURES
#endif /* DEBUG_BUILD */
} TextureID_t;

#endif /* _TEXTURE_ID_H_ */
