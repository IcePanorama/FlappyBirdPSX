#include <libetc.h>
#include <libgs.h>
#include <stdio.h>

#include "FB_DEFS.H"
#include "V_MAIN.H"

#define DIST_TO_SCREEN (512)

static ScreenBuffer_t screen_buffer[2];

int v_init(void) {
  // FIXME: need to determine NTSC/PAL and make changes accordingly
  SetVideoMode(0);
  GsInitGraph(SCREEN_WIDTH, SCREEN_HEIGHT, GsNONINTER | GsOFSGPU, 1, 0);

  FntLoad(960, 256);
  SetDumpFnt(FntOpen(0, 8, SCREEN_WIDTH, 64, 0, 512));
  SetGraphDebug(0);

  InitGeom();  // Init GTE
  SetGeomOffset(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  SetGeomScreen(DIST_TO_SCREEN);

  SetDefDrawEnv(&screen_buffer[0].draw_env, 0, 0, 320, 240);
  SetDefDrawEnv(&screen_buffer[1].draw_env, 0, 240, 320, 240);
  SetDefDispEnv(&screen_buffer[0].disp_env, 0, 240, 320, 240);
  SetDefDispEnv(&screen_buffer[1].disp_env, 0, 0, 320, 240);

  screen_buffer[0].draw_env.isbg = screen_buffer[1].draw_env.isbg = 1;
  setRGB0(&screen_buffer[0].draw_env, 0, 0, 0);
  setRGB0(&screen_buffer[1].draw_env, 0, 0, 0);

  screen_buffer[0].num_sprites = screen_buffer[1].num_sprites = 0;

  SetDispMask(1);
  return 0;
}

void v_loop(void) {
  static ScreenBuffer_t *curr_sb;
  u_long i;
  curr_sb = (curr_sb == screen_buffer) ? screen_buffer + 1 : screen_buffer;

  ClearOTag(curr_sb->ordering_table, OT_MAX_LEN);

  for (i = 0; i < curr_sb->num_sprites; i++) {
    AddPrim(&curr_sb->ordering_table[i], curr_sb->sprites[i]);
  }

  DrawSync(0);
  VSync(0);

  PutDrawEnv(&curr_sb->draw_env);
  PutDispEnv(&curr_sb->disp_env);

  ClearImage(&curr_sb->draw_env.clip, 0, 0, 0);

  DrawOTag(curr_sb->ordering_table);

  // reset sprites array
  curr_sb->num_sprites = 0;

  FntFlush(-1);
}

void add_sprite_to_sprites(POLY_F4 *sprite) {
  screen_buffer[0].sprites[screen_buffer[0].num_sprites] = sprite;
  screen_buffer[1].sprites[screen_buffer[1].num_sprites] = sprite;

  screen_buffer[0].num_sprites++;
  screen_buffer[1].num_sprites++;
}
