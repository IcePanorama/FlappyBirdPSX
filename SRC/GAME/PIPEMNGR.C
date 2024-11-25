#include "game/pipemngr.h"
#include "game/signals.h"
#include "sys/fb_bools.h"
#include "sys/fb_time.h"

#ifdef DEBUG_BUILD
#include <assert.h>
#include <stdio.h>
#endif /* DEBUG_BUILD */

#define MAX_NUM_PIPES (10)

static void destroy_pipe_entity (uint8_t u8_idx);
static void manage_pipe_entities (void);

PipesEntity_t pp_pipes[MAX_NUM_PIPES] = {{0}};
static uint8_t u8_num_pipes = 0;

//FIXME: should return 0 on success, non-zero on failure.
void
pm_init_pipe_manager (void)
{
  if (SetRCnt (RCntCNT2, 0xFFFF, RCntMdNOINTR | RCntMdFR) != 1)
  {
#ifdef DEBUG_BUILD
    printf ("ERROR: failed to init RCntCNT2 for PipeMngr.\n");
    assert(FALSE);
#endif /* DEBUG_BUILD */
  }

  memset (pp_pipes, 0, sizeof (PipesEntity_t) * MAX_NUM_PIPES);
  u8_num_pipes = 0;
}

void
pm_manage_pipes (void)
{
  /*
   *  FIXME: should `GetRCnt` be called after `manage_pipe_entities`?
   *  probably doesn't matter, but its worth considering!
   */
  uint16_t u16_curr_time = (GetRCnt (RCntCNT2) & 0xFFFF);
  static uint32_t u32_counter = 0;
  static uint16_t u16_last_time = 0;

  manage_pipe_entities ();

  /* Our counter has reset. */
  if (u16_last_time > u16_curr_time) u32_counter++;
  u16_last_time = u16_curr_time;

  if (u32_counter <= 10)
    return;

  pm_spawn_pipe_entity ();

  u32_counter = 0;
}

void
pm_spawn_pipe_entity ()
{
#ifdef DEBUG_BUILD
  assert((u8_num_pipes + 1) < MAX_NUM_PIPES);
#endif /* DEBUG_BUILD */

  pp_pipes[u8_num_pipes] = pie_create_pipes_entity ();
  u8_num_pipes++;
}

void
destroy_pipe_entity (uint8_t u8_idx)
{
  PipesEntity_t pe_tmp;

  /* if the given pipe is last active pipe, swapping isn't necessary. */
  if (u8_idx != (u8_num_pipes - 1))
  {
    pe_tmp = pp_pipes[u8_idx];
    pp_pipes[u8_idx] = pp_pipes[u8_num_pipes - 1];
    pp_pipes[u8_num_pipes - 1] = pe_tmp;
  }

  pie_destroy_pipes_entity (&pp_pipes[u8_num_pipes - 1]);
  u8_num_pipes--;
}

void
manage_pipe_entities ()
{
  uint8_t u8_pe_to_destroy[MAX_NUM_PIPES] = {0};
  uint8_t u8_num_destroyed = 0;
  uint8_t i;

  for (i = 0; i < u8_num_pipes; i++)
  {
    if ((si_check_inbox (pp_pipes[i].u8_eid)) == SIG_OFF_SCREEN_LEFT)
    {
#ifdef DEBUG_BUILD
      assert(u8_num_destroyed + 1 < MAX_NUM_PIPES);
#endif /* DEBUG_BUILD */

      u8_pe_to_destroy[u8_num_destroyed] = i;
      u8_num_destroyed++;
    }
  }

  for (i = 0; i < u8_num_destroyed; i++)
    destroy_pipe_entity (u8_pe_to_destroy[i]);
}

PipesEntity_t *
pm_get_pipe (uint8_t u8_eid)
{
  uint8_t i;
  for (i = 0; i < u8_num_pipes; i++)
  {
    if (pp_pipes[i].u8_eid == u8_eid)
      return &pp_pipes[i];
  }

#ifdef DEBUG_BUILD
  printf("Error: Pipe with entity id %d not found\n", u8_eid);
  assert(FALSE);
#endif /* DEBUG_BUILD */

  return 0;
}
