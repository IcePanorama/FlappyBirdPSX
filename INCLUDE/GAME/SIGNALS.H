/**
 *  SIGNALS - a teeny-tiny messaging system where each entity can have only one
 *  message at a time.
 */
#ifndef _SIGNALS_H_
#define _SIGNALS_H_

#include "sys/fb_ints.h"

typedef enum Signal_e
{
#ifndef DEBUG_BUILD
  SIG_NO_INBOX,  // it is impossible for `u8_entity_id` to recieve messages.
#endif /* DEBUG_BUILD */
  SIG_NO_SIGNALS, // `u8_entity_id` has an inbox, but no messages.
  SIG_OFF_SCREEN_LEFT,
  SIG_OFF_SCREEN_RIGHT,
  SIG_BELOW_SCREEN,
  SIG_ABOVE_SCREEN,
  SIG_NUM_SIGNALS
} Signal_t;

void si_init_signal_inboxes (void);
void si_send_signal (uint8_t u8_entity_id, Signal_t signal);
Signal_t si_check_inbox (uint8_t u8_entity_id);

#endif /* _SIGNALS_H_ */
