#ifndef _SCORE_H_
#define _SCORE_H_

#include "game_obj/player.h"
#include "sys/fb_bools.h"

/**
 *  Inits internal player pointer, calculates green zone x position. Must be
 *  called AFTER `ui_init_ui_elements`.
 */
void s_init_scoring (PlayerEntity_t *p);

/**
 *  Returns TRUE if x < -(player width + half pipe width). If TRUE, call
 *  `s_process_scoring` to handle the scoring stuff. MUST call `s_init_scoring`
 *  before use.
 *  TODO: Turn this into a macro function.
 */
bool_t s_in_green_area (int16_t x);

/**
 *  Increases player's score provided that the given pipe hasn't been counted
 *  towards the player's score already.
 */
void s_process_scoring (uint8_t u8_eid);

void s_process_high_scores (void);

#endif /* _SCORE_H_ */
