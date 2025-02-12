/** memcard.h - memory card i/o functions. */
#ifndef _MEMORY_CARDS_H_
#define _MEMORY_CARDS_H_

/**
 *  Initializes internal variables related to memory card events, and opens the
 *  events themselves.
 *  Returns: Zero on success, non-zero on failure.
 *  See: libovr, pp. 60-1.
 */
int mc_init (void);

/**
 *  Saves high scores to memory card. Enables memory card events, does the
 *  saving, and then disables those events between calls to this function.
 */
void mc_save_game (void);

#endif /* _MEMORY_CARDS_H_ */
