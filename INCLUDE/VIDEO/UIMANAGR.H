#ifndef _USER_INTERFACE_MANAGER_H_
#define _USER_INTERFACE_MANAGER_H_

#include "sys/fb_ints.h"

#include <sys/types.h>

typedef enum UIGameOverCoinState_e
{
  UICS_NO_COIN,
  UICS_SILVER_COIN,
  UICS_GOLD_COIN
#ifdef DEBUG_BUILD
, UICS_NUM_COIN_STATES
#endif /* DEBUG_BUILD */
} UIGameOverCoinState_t;

void ui_init_ui_elements (void);
void ui_draw_ui_elements (u_long *ot, u_long *ot_idx);
void ui_set_game_over_coin_state (UIGameOverCoinState_t state);
void ui_update_current_score_text (uint16_t u16_score);
void ui_update_high_score_text (uint16_t u16_high_score);

#endif /* _USER_INTERFACE_MANAGER_H_ */
