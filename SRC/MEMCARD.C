#include "memcard.h"

#include <kernel.h>
#include <libapi.h>
#include <libpad.h>
#include <sys/file.h>

// tmp
#define DEBUG_BUILD

#ifdef DEBUG_BUILD
  #include <stdio.h>
  #include <assert.h>
#endif /* DEBUG_BUILD */

#define CLEAR_EVENT() \
  { \
    TestEvent (sw_events[MCSwEV_COMPLETE]); \
    TestEvent (sw_events[MCSwEV_ERROR]); \
    TestEvent (sw_events[MCSwEV_TIMEOUT]); \
    TestEvent (sw_events[MCSwEV_NEW]);\
  }

typedef enum MemCardSwEvent_e
{
  MCSwEV_COMPLETE,
  MCSwEV_ERROR,
  MCSwEV_TIMEOUT,
  MCSwEV_NEW,
  MCswEV_NUM_EVENTS
} MemCardSwEvent_t;

static unsigned long sw_events[MCswEV_NUM_EVENTS] = {0};

/**
 *  Gets the memory cards set up for I/O operations.
 *  Returns: Zero on success, non-zero on failure.
 */
static int card_setup (void);
static long get_card_event (void);
/** Resets everything after I/O operations are complete. */
static void card_clean_up (void);

int
mc_init (void)
{
  memset (sw_events, 0, sizeof (unsigned long) * MCswEV_NUM_EVENTS);

  EnterCriticalSection();
  sw_events[MCSwEV_COMPLETE] = OpenEvent (SwCARD,    EvSpIOE, EvMdNOINTR, NULL);
  sw_events[MCSwEV_ERROR]    = OpenEvent (SwCARD,  EvSpERROR, EvMdNOINTR, NULL);
  sw_events[MCSwEV_TIMEOUT]  = OpenEvent (SwCARD, EvSpTIMOUT, EvMdNOINTR, NULL);
  sw_events[MCSwEV_NEW]      = OpenEvent (SwCARD,    EvSpNEW, EvMdNOINTR, NULL);
  ExitCriticalSection();

#ifdef DEBUG_BUILD
  assert (sw_events[MCSwEV_COMPLETE] != -1);
  assert (sw_events[MCSwEV_ERROR] != -1);
  assert (sw_events[MCSwEV_TIMEOUT] != -1);
  assert (sw_events[MCSwEV_NEW] != -1);
#else /* not DEBUG_BUILD */
  if ((sw_events[MCSwEV_COMPLETE] == -1) || (sw_events[MCSwEV_ERROR] == -1)
      || (sw_events[MCSwEV_TIMEOUT] == -1) || (sw_events[MCSwEV_NEW] == -1))
      return -1;  // Failed to open some event
#endif /* not DEBUG_BUILD */

  return 0;
}

void
mc_save_game (void)
{
  if (card_setup () != 0)
    goto clean_up;

  // save file i/o goes here!
  printf ("Saving...\n");

//TODO: actually save data to mc!

  printf ("Done!\n");

clean_up:
  card_clean_up ();
}

int
card_setup (void)
{
  long ret; //, file_desc;

#ifdef DEBUG_BUILD
  assert (EnableEvent (sw_events[MCSwEV_COMPLETE]) == 1);
  assert (EnableEvent (sw_events[MCSwEV_ERROR]) == 1);
  assert (EnableEvent (sw_events[MCSwEV_TIMEOUT]) == 1);
  assert (EnableEvent (sw_events[MCSwEV_NEW]) == 1);
#else /* not DEBUG_BUILD */
  if ((EnableEvent (sw_events[MCSwEV_COMPLETE]) != 1)
      || (EnableEvent (sw_events[MCSwEV_ERROR]) != 1)
      || (EnableEvent (sw_events[MCSwEV_TIMEOUT]) != 1)
      || (EnableEvent (sw_events[MCSwEV_NEW]) != 1))
    return -1; // Failed to enable some event.
#endif /* not DEBUG_BUILD */

  PadStopCom ();
  InitCARD (0);
  StartCARD ();
  _bu_init ();

#ifdef DEBUG_BUILD
  assert (_card_info (0x00) == 1);
#else /* not DEBUG_BUILD */
  if (_card_info (0x00) != 1) return -1;
#endif /* not DEBUG_BUILD */
  ret = get_card_event ();

  if ((ret == MCSwEV_ERROR) || (ret == MCSwEV_TIMEOUT))
    return -1;
  else if (ret == MCSwEV_NEW)
    {
      CLEAR_EVENT ();

#ifdef DEBUG_BUILD
      assert (_card_clear (0x00) == 1);
#else /* not DEBUG_BUILD */
      if (_card_clear (0x00) != 1) return -1;
#endif /* not DEBUG_BUILD */
      ret = get_card_event ();
    }

  CLEAR_EVENT ();

  // Deliver a TEST FORMAT request
#ifdef DEBUG_BUILD
  assert (_card_load (0x00) == 1);
#else /* not DEBUG_BUILD */
  if (_card_load (0x00) != 1) return -1;
#endif /* not DEBUG_BUILD */

//FIXME: Formatting should done be done automatically, ask user for permission.
  if (ret == MCSwEV_NEW)
    {
#ifdef DEBUG_BUILD
      printf ("Formatting card...\n");
      assert (_card_format (0x00) == 1);
      printf ("Done!\n");
#else /* not DEBUG_BUILD */
      if (_card_format (0x00) != 1) return -1;
#endif /* not DEBUG_BUILD */
    }

  return 0;
}

long
get_card_event (void)
{
  while (1)
    {
      if (TestEvent (sw_events[MCSwEV_COMPLETE]) == 1)
        return MCSwEV_COMPLETE;
      if (TestEvent (sw_events[MCSwEV_ERROR]) == 1)
        return MCSwEV_ERROR;
      if (TestEvent (sw_events[MCSwEV_TIMEOUT]) == 1)
        return MCSwEV_TIMEOUT;
      if (TestEvent (sw_events[MCSwEV_NEW]) == 1)
        return MCSwEV_NEW;
    }
}

void
card_clean_up (void)
{
#ifdef DEBUG_BUILD
  assert (DisableEvent (sw_events[MCSwEV_COMPLETE]) == 1);
  assert (DisableEvent (sw_events[MCSwEV_ERROR]) == 1);
  assert (DisableEvent (sw_events[MCSwEV_TIMEOUT]) == 1);
  assert (DisableEvent (sw_events[MCSwEV_NEW]) == 1);
#else /* not DEBUG_BUILD */
  DisableEvent (sw_events[MCSwEV_COMPLETE]);
  DisableEvent (sw_events[MCSwEV_ERROR]);
  DisableEvent (sw_events[MCSwEV_TIMEOUT]);
  DisableEvent (sw_events[MCSwEV_NEW]);
#endif /* not DEBUG_BUILD */

  /*
   *  Calling `StopCARD` here should require calling `StartPAD` right after
   *  (see: libref, pg. 178). Doing this, however, generates a "coprocessor
   *  unusable" exception (excode=0Bh).
   *
   *  If `StopCARD` is called without calling `StartPAD`, everything functions
   *  as expected. This is probably a weird dependency issue between various PSX
   *  subsystems, who secretly need to be initialized/used in a very particular
   *  order.
   *
   *  According to psx.dev discord users, calling `StopCARD`/`StartPAD` here
   *  isn't even strictly necessary; simply disabling the events alone should
   *  suffice (at least for a simple 2d game such as this).
   *
   *  Therefore, these two lines will remain commented-out for the time being.
   */
/*
  StopCARD ();
  StartPAD ();
*/
  PadStartCom ();
}
