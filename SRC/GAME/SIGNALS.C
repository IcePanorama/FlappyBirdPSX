#include "game/signals.h"

#ifdef DEBUG_BUILD
#include <assert.h>
#include <stdio.h>
#endif /* DEBUG_BUILD */

#define MAX_NUM_SIG_INBOXES (255)

Signal_t sig_inboxes[(MAX_NUM_SIG_INBOXES)];

void
si_init_signal_inboxes (void)
{
  memset (sig_inboxes, SIG_NO_SIGNALS,
          sizeof (Signal_t) * (MAX_NUM_SIG_INBOXES));
}

void
si_send_signal (uint8_t u8_entity_id, Signal_t signal)
{
#ifdef DEBUG_BUILD
  assert(u8_entity_id < (MAX_NUM_SIG_INBOXES));
#else /* DEBUG_BUILD */
  if (u8_entity_id >= (MAX_NUM_SIG_INBOXES))
    return;
#endif /* DEBUG_BUILD */

  sig_inboxes[u8_entity_id] = signal;
}

Signal_t
si_check_inbox (uint8_t u8_entity_id)
{
  Signal_t output;

#ifdef DEBUG_BUILD
  assert(u8_entity_id < (MAX_NUM_SIG_INBOXES));
#else /* DEBUG_BUILD */
  if (u8_entity_id >= (MAX_NUM_SIG_INBOXES))
    return SIG_NO_INBOX;
#endif /* DEBUG_BUILD */

  output = sig_inboxes[u8_entity_id];
  sig_inboxes[u8_entity_id] = SIG_NO_SIGNALS;
  return output;
}
