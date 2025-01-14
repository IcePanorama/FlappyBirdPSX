/** textmngr.h - Texture manager. Related functions are prefixed with 'tmg'. */
#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

/* Do not touch! CC seems really particular about the order of these. */
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libetc.h>
/******************/

#include "video/textid.h"

//idk if I like these being here.
extern DR_TPAGE tpages[TEXTID_NUM_TEXTURES];
extern DR_TPAGE player_tpage;

void tmg_auto_load_textures (void);

#endif /* _TEXTURE_MANAGER_H_ */
