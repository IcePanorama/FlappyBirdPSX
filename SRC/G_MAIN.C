#include "FB_DEFS.H"
#include "G_MAIN.H"
#include "G_MANGER.H"
#include "S_CONTRL.H"

void g_init(void) {
  player = g_create_player(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, controller_1);
  g_manager_init();
}

void g_loop(void) {
  g_player_loop(&player);
  g_manager_loop();
}
